#include "Coroutine.h"
#include <Adafruit_DotStar.h>

#define DOTSTAR_NUMPIXELS 1 
#define DOTSTAR_DATAPIN   7
#define DOTSTAR_CLOCKPIN  8

Adafruit_DotStar strip = Adafruit_DotStar(
  DOTSTAR_NUMPIXELS, DOTSTAR_DATAPIN, DOTSTAR_CLOCKPIN, DOTSTAR_BGR);

  
#define RED_LED_PIN 13
#define DMX_RX_PIN 3
#define DEBUG_PIN1 0
#define DEBUG_PIN2 1



void setup() {  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(DEBUG_PIN1, OUTPUT);
  pinMode(DEBUG_PIN2, OUTPUT);
  Serial1.begin(250000);
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}  


inline void Debug(int d)
{
   digitalWrite(DEBUG_PIN1, d / 2);
   digitalWrite(DEBUG_PIN2, d % 2);
}

volatile int val=-1;
void dmxLineISR()
{
  val=digitalRead(DMX_RX_PIN);  
}

void loop()
{
  //while(1)
  //  TRACE("%u", SysTick->VAL);
  
  int len;
  while(1)
  {
    Debug(0);
    val=-1;
    attachInterrupt(DMX_RX_PIN, dmxLineISR, CHANGE);
    while(val!=0);
    int t0 = micros();
    while(val!=1);
    int t1 = micros();
    int v1 = val;
    Debug(1);
    len = t1 - t0;
    if( len > 72 )
    {
      //TRACE("%dus", len);
      
      break;
    }    
  }
  detachInterrupt(DMX_RX_PIN);
  Serial1.clear_read();
  Serial1.begin(250000, SERIAL_8N2);
  //TRACE("await serial", len);
  Debug(3);  
  while(!Serial1.available());
  //TRACE("got serial", len);
  int b = Serial1.read();
  if( b != 0 )
    return; // not regular DMX

  int n = 512;
  int c[6];
  for(volatile int i=0; i<n; i++ )
  {
    while(!Serial1.available());
    int b = Serial1.read();
    if( i<(sizeof(c)/sizeof(c[0])) )
       c[i] = b;
  }
  Debug(2);      
  //TRACE("%dus: %d %d %d %d %d %d", len, c[0], c[1], c[2], c[3], c[4], c[5] );
  if( c[3] >= 128 )
      digitalWrite(RED_LED_PIN, HIGH);
  else 
      digitalWrite(RED_LED_PIN, LOW);
  strip.setPixelColor(0, (c[0]<<16) + (c[1]<<8) + c[2]);
  strip.show();
}
