/**
 * @file dimmer.ino
 * ### `gadget-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Example: DMX receiver with DotStar and SSD1306 display
 */

//#define LEVELS_TO_SSD1306
#define LEVELS_TO_DOTSTAR
//#define STACK_USAGE_TO_SERIAL
//#define SSD1306_EXAMPLE_AS_SUBSKETCH

#if defined(SSD1306_EXAMPLE_AS_SUBSKETCH) && defined(LEVELS_TO_SSD1306)
#error Must choose one usage for SSD1306 display driver
#endif

#include "Coroutine.h"
#include "Hopper.h"
#include "wiring_private.h"
#include "GC_Uart.h"


#ifdef SSD1306_EXAMPLE_AS_SUBSKETCH
// With no functional changes, I was able to get the example program from the SSD1306 
// display libaray to run in a coroutine very easily. The example program, ssd1306_128x32_i2c.ino
// contains long delays of a second or two via the `delay()` function. However, `delay()` yields
// so we can still get serived promptly when in foreground. Therefore, dimming remains smooth.
#include "SubSketch.h"
// We have to include everythign that the sub-sketch includes
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Now we enter a namespace for the sub-sketch's functions, including loop() and setup()
namespace display_subsketch 
{
// Include the sub-sketch. Modify this path to point to the appropriate ssd1306 
// example program. Note: I had to move the `setup()` function to the bottom of 
// ssd1306_128x32_i2c.ino so that the functions it calls would be defined.
#include "/home/jgraley/arduino/Arduino/libraries/Adafruit_SSD1306/examples/ssd1306_128x32_i2c/ssd1306_128x32_i2c.ino" 
}
// See SubSketch.h for info about this
GC_SUB_SKETCH_TASK(display_subsketch) display_subsketch_task;
#endif

#ifdef LEVELS_TO_SSD1306
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

#ifdef LEVELS_TO_DOTSTAR
#include <Adafruit_DotStar.h>
 
#define DOTSTAR_NUMPIXELS 1 
#define DOTSTAR_DATAPIN   7
#define DOTSTAR_CLOCKPIN  8

Adafruit_DotStar strip(DOTSTAR_NUMPIXELS, DOTSTAR_DATAPIN, DOTSTAR_CLOCKPIN, DOTSTAR_BGR);
#endif

#ifdef LEVELS_TO_SSD1306
#define SCREEN_WIDTH 128 // LEVELS_TO_SSD1306 display width, in pixels
#define SCREEN_HEIGHT 32 // LEVELS_TO_SSD1306 display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define LEVELS_TO_SSD1306_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, LEVELS_TO_SSD1306_RESET);
void display_bad_frame();
#endif

#define RED_LED_PIN 13
#define DMX_RX_PIN 3
#define DMX_BAUDRATE 250000

volatile bool enable_fg = true;

// Similar to what we get at the bottom of variant.cpp (for your platform), 
// Except that:
// 1. We use the INTERRUPT_HANDLER to generate an interrupt handler.
// 2. This interrupt handler forwards throuh a vector that is non_const.
// 3. Name of vector is just handler name with _ptr appended.
// 4. We construct a GC::Uart instead of a Uart, and pass in the vector.
// 5. You'll have to comment out ISR and Uart declrations in variant.h/cpp
INTERRUPT_HANDLER(SERCOM0_Handler)
GC::Uart Serial1(&sercom0, get_SERCOM0_Handler(), PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX);


// With coroutines, it's often more natural to set something
// up just before you need it.
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
GC::Uart::Error serial_error;


GC::Coroutine dmx_task([]
{
  GC::Hopper fg( []{ enable_fg=true; },
                 []{ enable_fg=false; } );                             
                 
  pinMode(RED_LED_PIN, OUTPUT);
#ifdef LEVELS_TO_DOTSTAR
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
#endif
#ifdef LEVELS_TO_SSD1306
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

    if( serial_error & GC::Uart::FRAME_ERROR )
    {
      digitalWrite(RED_LED_PIN, HIGH);
#ifdef LEVELS_TO_SSD1306
      display_bad_frame();
#else
      TRACE("frame error" );
#endif      
      continue;
    }
  
    if( start_code == 0 )
    {
      output_dmx_frame();
    }
#if defined(STACK_USAGE_TO_SERIAL) && !defined(LEVELS_TO_SSD1306)
    TRACE("CLS %d Stack %d", me()->get_cls_usage(), me()->estimate_stack_peak_usage());
#endif
    yield();
  }
});

void wait_for_break_pulse()
{
  // "Hop" on to the pin interrupt
  GC::Hopper hopper( []{ attachInterrupt(DMX_RX_PIN, *me(), CHANGE); },
                     []{ detachInterrupt(DMX_RX_PIN); } );                             

  int len;
  do
  {
    GC::Coroutine::wait( []{ return digitalRead(DMX_RX_PIN)==0; } );
    int t0 = my_micros();
    
    GC::Coroutine::wait( []{ return digitalRead(DMX_RX_PIN)==1; } );
    int t1 = my_micros();
      
    len = t1 - t0;
  } while( len < 72 );
}


void get_frame_data()
{
  // "Hop" across to UART interrupt
  GC::Hopper hopper( []{ Serial1.begin(250000, SERIAL_8N2); }, 
                     []{ Serial1.end(); } ); 

  start_code = Serial1.read(&serial_error);
  if( serial_error )
    return;
  if( start_code != 0 )
    return;
  
  for( int i=0; i<(int)sizeof(dmx_frame); i++ )
  {
    dmx_frame[i] = Serial1.read(&serial_error);
    if( serial_error )
      return;
  }
}


void get_dmx_frame()
{
  wait_for_break_pulse();             
  get_frame_data();        
}


#ifdef LEVELS_TO_SSD1306
void display_levels()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  char buf[256];
  sprintf(buf, "%02X%02X%02X %3d", dmx_frame[0], dmx_frame[1], dmx_frame[2], dmx_frame[3]);
  display.println(buf);
#ifdef STACK_USAGE_TO_SERIAL
  sprintf(buf, "C%d S%d", me()->get_cls_usage(), me()->estimate_stack_peak_usage());
  display.println(buf);
#endif  
  display.display(); 
}


void display_bad_frame()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  char buf[256];
  sprintf(buf, "Frame Err!");
  display.println(buf);
#ifdef STACK_USAGE_TO_SERIAL
  sprintf(buf, "C%d S%d", me()->get_cls_usage(), me()->estimate_stack_peak_usage());
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
#ifdef LEVELS_TO_DOTSTAR
  strip.setPixelColor(0, (dmx_frame[0]<<16) + (dmx_frame[1]<<8) + dmx_frame[2]);
  strip.show();
#endif
#ifdef LEVELS_TO_SSD1306
  display_levels();
#endif    
}


void loop()
{
  if( enable_fg )
  {
    dmx_task();
  }
  system_idle_tasks();
#ifdef SSD1306_EXAMPLE_AS_SUBSKETCH
  display_subsketch_task();
#endif
}
