#include "GCoroutines.cpp"
#include "GCoroutines.h"

void led_flasher_main(GCoroutine *gc)
{
  while(1)
  {
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(500);               // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      delay(500);               // wait for a second
      Serial.println("Tock!!");
      gc->yield(); // @TODO call a global function that uses cls to get to the object
  }
}

GCoroutine led_flasher( led_flasher_main );

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);  
}

// the loop function runs over and over again forever
void loop() {
  led_flasher.run_iteration();
}
