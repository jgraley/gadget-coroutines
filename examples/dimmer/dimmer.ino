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
#define DMX_BAUDRATE 250000

volatile unsigned char dmx_frame[513];
volatile int dmx_frame_index=0;


#ifdef USE_SERCOM

#define DMX_SERCOM sercom0
// Note: we are coding direct to the SERCOM API since we want to implement the ISR ourselves
void begin_serial()
{
  // PIN_SERIAL1_RX, PIN_SERIAL1_TX, PAD_SERIAL1_RX, PAD_SERIAL1_TX, NO_RTS_PIN, NO_CTS_PIN
  pinPeripheral(PIN_SERIAL1_RX, g_APinDescription[PIN_SERIAL1_RX].ulPinType);
  pinPeripheral(PIN_SERIAL1_TX, g_APinDescription[PIN_SERIAL1_TX].ulPinType);

  DMX_SERCOM->initUART(UART_INT_CLOCK, SAMPLE_RATE_x16, DMX_BAUDRATE);
  DMX_SERCOM->initFrame(UART_CHAR_SIZE_8_BITS, LSB_FIRST, SERCOM_NO_PARITY, SERCOM_STOP_BITS_2);
  DMX_SERCOM->initPads(PAD_SERIAL1_TX, PAD_SERIAL1_RX);

  DMX_SERCOM->enableUART();
}

void SERCOM0_Handler()  
{
  if (DMX_SERCOM->isFrameErrorUART()) {
    // frame error, next byte is invalid so read and discard it
    DMX_SERCOM->readDataUART();

    DMX_SERCOM->clearFrameErrorUART();
  }

  if (DMX_SERCOM->availableDataUART()) {
    unsigned char b = sercom->readDataUART();
    if( dmx_frome_index < sizeof(dmx_frame) )
      dmx_frame[dmx_frame_index++] = b;
  }

  /*
   if (DMX_SERCOM->isDataRegisterEmptyUART()) {
      DMX_SERCOM->writeDataUART(data);
    } else {
      DMX_SERCOM->disableDataRegisterEmptyInterruptUART();
    }
  }*/

  if (DMX_SERCOM->isUARTError()) {
    DMX_SERCOM->acknowledgeUARTError();
    // TODO: if (sercom->isBufferOverflowErrorUART()) ....
    // TODO: if (sercom->isParityErrorUART()) ....
    DMX_SERCOM->clearStatusUART();
  }
}
#endif // USE_SERCOM

void setup() {  
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(DEBUG_PIN1, OUTPUT);
  pinMode(DEBUG_PIN2, OUTPUT);
  Serial1.begin(DMX_BAUDRATE);
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
#ifdef USE_SERCOM
  begin_serial()
#else  
  Serial1.clear_read();
  Serial1.begin(250000, SERIAL_8N2);
#endif  
  
  Debug(3);  

#ifdef USE_SERCOM
  for( dmx_frame_index=0; dmx_frame_index<sizeof(dmx_frame); );
#else
  for( dmx_frame_index=0; dmx_frame_index<sizeof(dmx_frame); dmx_frame_index++ )
  {
    while(!Serial1.available());
    int b = Serial1.read();
    dmx_frame[dmx_frame_index] = b;
  }
#endif

  Debug(2);      

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
