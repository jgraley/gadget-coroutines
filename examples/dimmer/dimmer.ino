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

volatile unsigned char dmx_frame[513];
volatile int dmx_frame_index=0;
volatile bool enable_fg = true;

SERCOM *dmx_sercom = &sercom0;
// Note: we are coding direct to the SERCOM API since we want to implement the ISR ourselves

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

void read_byte_from_uart()
{
  if (dmx_sercom->isUARTError()) {
    
    handle_UART_error();    
  }

  if (dmx_sercom->availableDataUART()) {
    unsigned char b = dmx_sercom->readDataUART();
    if( dmx_frame_index < (int)sizeof(dmx_frame) )
      dmx_frame[dmx_frame_index++] = b;
  }
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

void what_was_loop()
{
  int len;

  Debug(0);
  
  // "Hop" on to the pin interrupt
  enable_fg=false; 
  Coroutine::yield([](){ attachInterrupt(DMX_RX_PIN, dmxLineISR, CHANGE); }); 
  while(1)
  {
    Debug(0);

    while(digitalRead(DMX_RX_PIN)!=0)
      yield();   

    int t0 = my_micros();
    yield();
    
    while(digitalRead(DMX_RX_PIN)!=1)
      yield();
  
    int t1 = my_micros();
    
    Debug(1);
   
    len = t1 - t0;
    if( len >= 72 )
    {
      break;
    }    
    yield();
  }
  detachInterrupt(DMX_RX_PIN);
  // "Hop" to UART interrupt
  Coroutine::yield([](){ dmx_uart_shutdown();
                         dmx_uart_claim_pins();
                         dmx_uart_init(); }); 
  frame_error = false;

  Debug(3);  

  // Wait for the SERCOM ISR to fill the buffer
  for( dmx_frame_index=0; dmx_frame_index<(int)sizeof(dmx_frame) && !frame_error; )
  {
    read_byte_from_uart();
    if( dmx_frame_index<(int)sizeof(dmx_frame) && !frame_error)
      yield();    // yield if will iterate again (i.e. need another byte)
  }

  Debug(frame_error?2:3);      

  dmx_uart_shutdown();
  // "Hop" back to foreground
  Coroutine::yield([](){ enable_fg=true; });   

  if( frame_error )
    return;

  if( dmx_frame[0] != 0 )
    return; // not regular DMX

  auto *c = dmx_frame+1;
  
  //TRACE("%dus: %d %d %d %d %d %d", len, c[0], c[1], c[2], c[3], c[4], c[5] );
  if( c[3] >= 128 )
      digitalWrite(RED_LED_PIN, HIGH);
  else 
      digitalWrite(RED_LED_PIN, LOW);
  strip.setPixelColor(0, (c[0]<<16) + (c[1]<<8) + c[2]);
  strip.show();
}


Coroutine dmx_loop([]{
  while(1)
  {
    what_was_loop();
    yield();
  }
});


void loop()
{
  if( enable_fg )
    dmx_loop();
  if( frame_error )
  {
    TRACE("frame error" );
  }
}


void dmxLineISR()
{
  dmx_loop();
}


void SERCOM0_Handler()  
{
  dmx_loop();
}
