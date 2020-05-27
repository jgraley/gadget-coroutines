#include "Coroutines.h"
#include <memory>

using namespace std;

shared_ptr<Coroutine> led_flasher; 

// the setup function runs once when you press reset or power the board
void setup() {
  // Seem to need to wait a little before the serial output actually appears
  delay(2000); // 2s
  
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  Serial.begin(9600);  
  
  // Construct in setup() so that we can use the serial output
  Serial.println("Constructing...");
  led_flasher = make_shared<Coroutine>([]()
  {
    while(1)
    {
      // Flash LED to show we're alive, but no ticks or tocks!
      digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(500);               // wait for a second
      digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
      delay(500);               // wait for a second
    }
  });
  Serial.println("Construct complete");

  // Do one iteration of the run_iteration to complete the initialisation
  Serial.println("First iteration...");
  (*led_flasher)();
  Serial.println("First iteration complete");
}

// the loop function runs over and over again forever
void loop() {
  delay(154);
  (*led_flasher)();
}
