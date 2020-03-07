#include "Coroutines.h"

Coroutine led_flasher( []()
{
  while(1)
  {
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(500);               // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      delay(500);               // wait for a second
      Serial.println("Tock!!");
      Coroutine::yield(); 
  }
} );

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);  
}

// the loop function runs over and over again forever
void loop() {
  Serial.println("Tick!!");
  led_flasher.run_iteration();
}

// Include at end to catch unexpected dependencies
#include "Coroutines.cpp"
