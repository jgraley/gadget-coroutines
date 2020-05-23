#include "Coroutine.h"

#define LED_PIN 13

#define CPU_HZ 48000000
#define TIMER_PRESCALER_DIV 1024

void startTimer(int frequencyHz);
void setTimerFrequency(int frequencyHz);
volatile bool enable_fg = true;

#include "sam.h"
extern volatile DeviceVectors exception_table;

INTERRUPT_HANDLER(TC3_Handler)


Coroutine led_flasher([]()
{
  TcCount16* TC = (TcCount16*) TC3;
  while(1)
  {
    if( random(2) )
    {
      // Emit a flash "on" the interrupt
      digitalWrite(LED_PIN, true);
  
      // "Hop" on to the interrupt
      enable_fg=false; 
      me()->set_hop_lambda([](){ Attach_TC3_Handler(*me()); NVIC_EnableIRQ(TC3_IRQn); });
      yield(); 
      TC->INTFLAG.bit.MC0 = 1;
  
      digitalWrite(LED_PIN, false);
  
      Coroutine::yield(); 
      TC->INTFLAG.bit.MC0 = 1;
    }
    else
    {
      // Emit a flash "on" the loop() function
      digitalWrite(LED_PIN, true);
  
      // "Hop" back to foreground
      NVIC_DisableIRQ(TC3_IRQn);
      Detach_TC3_Handler();
      me()->set_hop_lambda([](){ enable_fg=true; }); 
      yield(); 
  
      digitalWrite(LED_PIN, false);
  
      Coroutine::yield(); 
    }
  }
});


void setup() {
  pinMode(LED_PIN, OUTPUT);
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
    led_flasher();
  }
  int n = random(3000, 100000);
  for(volatile int i=0; i<n; i++ )
  {
      system_idle_tasks();
  }
}
