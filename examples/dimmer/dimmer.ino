#include "Coroutine.h"
#include <Adafruit_DotStar.h>
#include "wiring_private.h"

#define DOTSTAR_NUMPIXELS 1 
#define DOTSTAR_DATAPIN   7
#define DOTSTAR_CLOCKPIN  8

Adafruit_DotStar strip = Adafruit_DotStar(
  DOTSTAR_NUMPIXELS, DOTSTAR_DATAPIN, DOTSTAR_CLOCKPIN, DOTSTAR_BGR);
  
#define RED_LED_PIN 13
#define DMX_RX_PIN 3
#define DEBUG_PIN1 0
#define DEBUG_PIN2 1
#define DMX_BAUDRATE 250000

volatile bool enable_fg = true;

SERCOM *dmx_sercom = &sercom0;
// Note: we are coding direct to the SERCOM API since we want to implement the ISR ourselves

void (*SERCOM0_Handler_ptr)();

void SERCOM0_Handler()  
{
  if( SERCOM0_Handler_ptr )
    SERCOM0_Handler_ptr();
}

void dmx_uart_init()
{
  // PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX, NO_RTS_PIN, NO_CTS_PIN
  dmx_sercom->initUART(UART_INT_CLOCK, SAMPLE_RATE_x16, DMX_BAUDRATE);
  dmx_sercom->initFrame(UART_CHAR_SIZE_8_BITS, LSB_FIRST, SERCOM_NO_PARITY, SERCOM_STOP_BITS_2);
  dmx_sercom->initPads(PAD_SERIAL1_TX, PAD_SERIAL1_RX);

  dmx_sercom->enableUART();
}

void dmx_uart_shutdown()
{
  dmx_sercom->resetUART();
}

void dmx_uart_claim_pins()
{
  // PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX, NO_RTS_PIN, NO_CTS_PIN
  pinPeripheral(PIN_SERIAL1_RX, g_APinDescription[PIN_SERIAL1_RX].ulPinType);
  pinPeripheral(PIN_SERIAL1_TX, g_APinDescription[PIN_SERIAL1_TX].ulPinType);
}

volatile bool frame_error = false;

void handle_UART_error()
{
  dmx_sercom->acknowledgeUARTError();
  if (dmx_sercom->isFrameErrorUART()) {
    // frame error, next byte is invalid so read and discard it
    dmx_sercom->readDataUART();
    frame_error = true;

    dmx_sercom->clearFrameErrorUART();
  }  
  // TODO: if (sercom->isBufferOverflowErrorUART()) ....
  // TODO: if (sercom->isParityErrorUART()) ....
  dmx_sercom->clearStatusUART();
}

uint8_t read_byte_from_uart()
{
  Coroutine::wait( []{ return dmx_sercom->isUARTError() || dmx_sercom->availableDataUART(); } );

  if(dmx_sercom->isUARTError())
  {
    handle_UART_error();  
    return 0;
  }
  
  return dmx_sercom->readDataUART();
}


void setup() {  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(DEBUG_PIN1, OUTPUT);
  pinMode(DEBUG_PIN2, OUTPUT);
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}  


inline void Debug(int d)
{
   digitalWrite(DEBUG_PIN1, d / 2);
   digitalWrite(DEBUG_PIN2, d % 2);
}

extern volatile uint32_t _ulTickCount;
unsigned long my_micros( void )
{
  uint32_t ticks  = SysTick->VAL;
  uint32_t pend   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
  uint32_t count  = _ulTickCount ;
  return ((count+pend) * 1000) + (((SysTick->LOAD  - ticks)*(1048576/(VARIANT_MCK/1000000)))>>20) ;
}


class RAIIHopper
{
public:
  RAIIHopper( std::function<void()> ext_detach_, std::function<void()> my_attach_, std::function<void()> my_detach_, std::function<void()> ext_attach_ ) :
    my_detach( my_detach_ ),
    ext_attach( ext_attach_ )
  {
    ext_detach_();
    me()->set_hop_lambda( my_attach_ );
  }

  void hop(std::function<void()> my_attach_, std::function<void()> my_detach_)
  {
    my_detach();
    me()->set_hop_lambda( my_attach_ );
    my_detach = my_detach_;
  }

  ~RAIIHopper()
  {
    my_detach();
    me()->set_hop_lambda( ext_attach );
  }
private: 
  std::function<void()> my_detach;                             
  std::function<void()> ext_attach;
};


void output_dmx_frame();
void get_dmx_frame();
uint8_t start_code;
uint8_t dmx_frame[512];
Coroutine dmx_loop([]{
  while(1)
  {
    get_dmx_frame();
    yield();

    if( frame_error )
    {
      digitalWrite(RED_LED_PIN, HIGH);
      continue;
    }
  
    if( start_code == 0 )
      output_dmx_frame();
      
    yield();
  }
});

void wait_for_break_pulse()
{
  // "Hop" on to the pin interrupt
  RAIIHopper hopper( []{ enable_fg=false; },
                     []{ attachInterrupt(DMX_RX_PIN, dmx_loop, CHANGE); },
                     []{ detachInterrupt(DMX_RX_PIN); },
                     []{ enable_fg=true; } );                             

  int len;
  do
  {
    Coroutine::wait( []{ return digitalRead(DMX_RX_PIN)==0; } );
    int t0 = my_micros();
    
    Coroutine::wait( []{ return digitalRead(DMX_RX_PIN)==1; } );
    int t1 = my_micros();
      
    len = t1 - t0;
  } while( len < 72 );
}


void get_frame_data()
{
  // "Hop" across to UART interrupt
  RAIIHopper hopper( []{ enable_fg=false; },
                     []{ dmx_uart_shutdown();
                         dmx_uart_claim_pins();
                         dmx_uart_init();
                         SERCOM0_Handler_ptr=dmx_loop;}, 
                     []{ dmx_uart_shutdown();
                         SERCOM0_Handler_ptr=nullptr;},
                     []{ enable_fg=true; } ); 

  Debug(3);  

  frame_error = false;
  start_code = read_byte_from_uart();
  if( frame_error )
    return;
  if( start_code != 0 )
    return;
  
  for( int i=0; i<(int)sizeof(dmx_frame); i++ )
  {
    dmx_frame[i] = read_byte_from_uart();
    if( frame_error )
      return;
  }
}


void get_dmx_frame()
{
  Debug(0);

  //strange();
  wait_for_break_pulse();             
  get_frame_data();   
  
  Debug(frame_error?2:3);      
}


void output_dmx_frame()
{
  //TRACE("%dus: %d %d %d %d %d %d", len, dmx_frame[0], dmx_frame[1], dmx_frame[2], dmx_frame[3], dmx_frame[4], dmx_frame[5] );
  if( dmx_frame[3] >= 128 )
      digitalWrite(RED_LED_PIN, HIGH);
  else 
      digitalWrite(RED_LED_PIN, LOW);
  strip.setPixelColor(0, (dmx_frame[0]<<16) + (dmx_frame[1]<<8) + dmx_frame[2]);
  strip.show();
}

void loop()
{
  if( enable_fg )
  {
    //dmx_loop();
    ((void(*)())dmx_loop)(); // invoke via Super Functor
  }
  if( frame_error )
  {
    TRACE("frame error" );
  }
  system_idle_tasks();
}
