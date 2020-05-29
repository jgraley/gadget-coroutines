/**
 * @file hopping.ino
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Example: Flashing LEDs with hopping onto Timer ISR
 */
 
#define USE_DOTSTAR

#include "Coroutine.h"
#include "Hopper.h"
#ifdef USE_DOTSTAR
#include <Adafruit_DotStar.h>

#define DOTSTAR_NUMPIXELS 1 
#define DOTSTAR_DATAPIN   7
#define DOTSTAR_CLOCKPIN  8

Adafruit_DotStar strip(DOTSTAR_NUMPIXELS, DOTSTAR_DATAPIN, DOTSTAR_CLOCKPIN, DOTSTAR_BGR);
#endif

#define RED_LED_PIN 13

#define CPU_HZ 48000000
#define TIMER_PRESCALER_DIV 1024

void startTimer(int frequencyHz);
void setTimerFrequency(int frequencyHz);
volatile bool enable_fg = true;

#include "sam.h"
extern volatile DeviceVectors exception_table;

INTERRUPT_HANDLER(TC3_Handler)


HC::Coroutine led_flasher_task([]()
{
  HC::Hopper fg( []{ enable_fg=true; },
                 []{ enable_fg=false; } );                             

#ifdef USE_DOTSTAR
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
#endif

  TcCount16* TC = (TcCount16*) TC3;
  while(1)
  {
    if( random(2) )
    {
      // "Hop" on to the interrupt
      HC::Hopper hopper( []{ *get_TC3_Handler() = *me(); NVIC_EnableIRQ(TC3_IRQn); },
                         []{ NVIC_DisableIRQ(TC3_IRQn); *get_TC3_Handler() = nullptr; } );      
      yield(); // when this returns we're in ISR 
      TC->INTFLAG.bit.MC0 = 1; // Ack the interrupt
      
      // Emit a flash "on" the interrupt
#ifdef USE_DOTSTAR
      strip.setPixelColor(0, 10<<8);
      strip.show();
#else
      digitalWrite(RED_LED_PIN, true);
#endif
  
      yield(); // await next interrupt
      TC->INTFLAG.bit.MC0 = 1; // Ack the interrupt
  
#ifdef USE_DOTSTAR
      strip.setPixelColor(0, 0);
      strip.show();
#else
      digitalWrite(RED_LED_PIN, false);
#endif
      // First yield after this block will "hop" back to foreground      
    }
    else
    {
      yield(); // definiterly in foreground when this returns
      
      // Emit a flash "on" foreground
      digitalWrite(RED_LED_PIN, true);
  
      yield(); 
  
      digitalWrite(RED_LED_PIN, false);
  
      HC::Coroutine::yield(); 
    }
  }
});


void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  startTimer(10);
}

void setTimerFrequency(int frequencyHz) {
  int compareValue = (CPU_HZ / (TIMER_PRESCALER_DIV * frequencyHz)) - 1;
  TcCount16* TC = (TcCount16*) TC3;
  // Make sure the count is in a proportional position to where it was
  // to prevent any jitter or disconnect when changing the compare value.
  TC->COUNT.reg = map(TC->COUNT.reg, 0, TC->CC[0].reg, 0, compareValue);
  TC->CC[0].reg = compareValue;
  Serial.println(TC->COUNT.reg);
  Serial.println(TC->CC[0].reg);
  while (TC->STATUS.bit.SYNCBUSY == 1);
}

void startTimer(int frequencyHz) {
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TCC2_TC3) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync

  TcCount16* TC = (TcCount16*) TC3;

  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Use the 16-bit timer
  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Use match mode so that the timer counter resets when the count matches the compare register
  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  // Set prescaler to 1024
  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync

  setTimerFrequency(frequencyHz);

  // Enable the compare interrupt
  TC->INTENSET.reg = 0;
  TC->INTENSET.bit.MC0 = 1;

  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
}


void loop()
{
  if( enable_fg )
  {
    led_flasher_task();
  }
  int n = random(3000, 100000);
  for(volatile int i=0; i<n; i++ )
  {
      system_idle_tasks();
  }
}
