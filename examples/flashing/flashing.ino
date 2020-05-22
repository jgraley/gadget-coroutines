
#include "Coroutine.h"

#include <Adafruit_DotStar.h>

#define DOTSTAR_NUMPIXELS 1 
#define DOTSTAR_DATAPIN   7
#define DOTSTAR_CLOCKPIN  8

Adafruit_DotStar strip = Adafruit_DotStar(
  DOTSTAR_NUMPIXELS, DOTSTAR_DATAPIN, DOTSTAR_CLOCKPIN, DOTSTAR_BGR);

__thread int tls_var;
int global_var;

  
Coroutine led_flasher([]()
{
  int  local_var;
  while(1)
  {
    //TRACE("tls_var LED flasher %p global %p local %p", &tls_var, &global_var, &local_var);
    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(500);               // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(500);               // wait for a second
    Serial.println(tls_var);
    tls_var += 3;
  }
}); 


Coroutine dotstar_flasher([]()
{
  while(1)
  {
    strip.setPixelColor(0, 10<<16); // dim red
    strip.show();
    delay(342);               // wait for a second
    strip.setPixelColor(0, 0); // off
    strip.show();
    delay(290);               // wait for a second
    Serial.println(tls_var);
    tls_var += 10;    
  }
}); 


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  //Serial.begin(9600);  
  delay(2000); // 2s

}


// the loop function runs over and over again forever
void loop() {
  int local_var;
  delay(154);
  Serial.println(tls_var);
  tls_var += 7;
  //TRACE("tls_var outside %p global %p local %p", &tls_var, &global_var, &local_var);
  led_flasher();
  dotstar_flasher();
  system_idle_tasks();
}