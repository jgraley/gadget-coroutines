#define OLED
#define DOTSTAR
#define STACK_USAGE

// With no functional changes, I was able to get the example program from the SSD1306 
// display libaray to run in a coroutine very easily. The example program, ssd1306_128x32_i2c.ino
// contains long delays of a second or two via the `delay()` function. However, `delay()` yields
// so we can still get serived promptly when in foreground. Therefore, dimming remains smooth.
//#define SSD1306_EXAMPLE_AS_SUBSKETCH

#include "Coroutine.h"
#include "Hopper.h"
#include "wiring_private.h"

#ifdef OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

#ifdef DOTSTAR
#include <Adafruit_DotStar.h>
 
#define DOTSTAR_NUMPIXELS 1 
#define DOTSTAR_DATAPIN   7
#define DOTSTAR_CLOCKPIN  8

Adafruit_DotStar strip(DOTSTAR_NUMPIXELS, DOTSTAR_DATAPIN, DOTSTAR_CLOCKPIN, DOTSTAR_BGR);
#endif

#ifdef OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

#define RED_LED_PIN 13
#define DMX_RX_PIN 3
#define DMX_BAUDRATE 250000

volatile bool enable_fg = true;

SERCOM *dmx_sercom = &sercom0;
// Note: we are coding direct to the SERCOM API since we want to implement the ISR ourselves

INTERRUPT_HANDLER(SERCOM0_Handler)

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


void setup() 
{  
}  


extern volatile uint32_t _ulTickCount;
unsigned long my_micros( void )
{
  uint32_t ticks  = SysTick->VAL;
  uint32_t pend   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
  uint32_t count  = _ulTickCount ;
  return ((count+pend) * 1000) + (((SysTick->LOAD  - ticks)*(1048576/(VARIANT_MCK/1000000)))>>20) ;
}


void output_dmx_frame();
void get_dmx_frame();
uint8_t start_code;
uint8_t dmx_frame[512];


Coroutine dmx_task([]
{
  Hopper fg( []{ enable_fg=true; },
             []{ enable_fg=false; } );                             
                 
  pinMode(RED_LED_PIN, OUTPUT);
#ifdef DOTSTAR
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
#endif
#ifdef OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))  // Address 0x3D for 128x64
  { 
    TRACE("SSD1306 allocation failed");
    return;
  }
#endif
  while(1)
  {
    get_dmx_frame();
    yield();

    if( frame_error )
    {
      TRACE("frame error" );
      digitalWrite(RED_LED_PIN, HIGH);
      continue;
    }
  
    if( start_code == 0 )
    {
      output_dmx_frame();
    }
#if defined(STACK_USAGE) && !defined(OLED)
    TRACE("TLS %d Stack %d", me()->get_tls_usage(), me()->estimate_stack_peak_usage());
#endif
    yield();
  }
});

void wait_for_break_pulse()
{
  // "Hop" on to the pin interrupt
  Hopper hopper( []{ attachInterrupt(DMX_RX_PIN, *me(), CHANGE); },
                 []{ detachInterrupt(DMX_RX_PIN); } );                             

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
  Hopper hopper( []{ dmx_uart_shutdown();
                     dmx_uart_claim_pins();
                     dmx_uart_init();
                     Attach_SERCOM0_Handler(*me());}, 
                 []{ dmx_uart_shutdown();
                     Detach_SERCOM0_Handler();} ); 

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
  wait_for_break_pulse();             
  get_frame_data();        
}


#ifdef OLED
void display_levels(Adafruit_SSD1306 &display)
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  char buf[256];
  sprintf(buf, "%02X%02X%02X %3d", dmx_frame[0], dmx_frame[1], dmx_frame[2], dmx_frame[3]);
  display.println(buf);
#ifdef STACK_USAGE
  sprintf(buf, "T%d S%d", me()->get_tls_usage(), me()->estimate_stack_peak_usage());
  display.println(buf);
#endif  
  display.display(); 
}
#endif

void output_dmx_frame()
{
  //TRACE("%dus: %d %d %d %d %d %d", len, dmx_frame[0], dmx_frame[1], dmx_frame[2], dmx_frame[3], dmx_frame[4], dmx_frame[5] );
  if( dmx_frame[3] >= 128 )
      digitalWrite(RED_LED_PIN, HIGH);
  else 
      digitalWrite(RED_LED_PIN, LOW);
#ifdef DOTSTAR
  strip.setPixelColor(0, (dmx_frame[0]<<16) + (dmx_frame[1]<<8) + dmx_frame[2]);
  strip.show();
#endif
#ifdef OLED
  display_levels(display);
#endif    
}


#ifdef SSD1306_EXAMPLE_AS_SUBSKETCH
#define setup subsketch_setup
#define loop subsketch_loop
// Modify this path to point to the appropriate ssd1306 example program.
// Note: I had to move the `setup()` function to the bottom of ssd1306_128x32_i2c.ino 
// so that the functions it calls would be defined.
#include "/home/jgraley/arduino/Arduino/libraries/Adafruit_SSD1306/examples/ssd1306_128x32_i2c/ssd1306_128x32_i2c.ino"
#undef setup
#undef loop
Coroutine display_subsketch([]
{
  subsketch_setup(); 
  while(1) 
    subsketch_loop();
});
#endif

void loop()
{
  if( enable_fg )
  {
    dmx_task();
  }
  system_idle_tasks();
#ifdef SSD1306_EXAMPLE_AS_SUBPROGRAM
  display_subprogram();
#endif
}
