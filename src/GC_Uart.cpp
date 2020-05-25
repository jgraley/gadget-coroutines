/**
 * GC_Uart.cpp
 * gadget-coroutines
 * Stacked coroutines for the Arduino environment.
 * (C) 2020 John Graley; BSD license applies.
 */

#include "GC_Uart.h"

#include <functional>
#include <atomic>

using namespace std;
using namespace GC;

GC_Uart::GC_Uart(SERCOM *_s, void (**_vector)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX) :
  Uart( _s, _pinRX, _pinTX, _padRX, _padTX ),
  sercom( _s ),
  vector( _vector )
{
}
 
  
GC_Uart::GC_Uart(SERCOM *_s, void (**_vector)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinRTS, uint8_t _pinCTS) :
  Uart( _s, _pinRX, _pinTX, _padRX, _padTX, _pinRTS, _pinCTS ),
  sercom( _s ),
  vector( _vector )
{
}


void GC_Uart::begin(unsigned long baudRate)
{
  *vector = *me();
  Uart::begin(baudRate);
}


void GC_Uart::begin(unsigned long baudrate, uint16_t config)
{
  *vector = *me();
  Uart::begin(baudrate, config);
}


void GC_Uart::end()
{
  Uart::end();
  *vector = nullptr;
}


int GC_Uart::read( Error *error_p )
{
  if( error_p )
    *error_p = NO_ERROR;
  Coroutine::wait( [=]{ return sercom->isUARTError() || sercom->availableDataUART(); } );

  if(sercom->isUARTError())
  {
    handle_UART_error(error_p);  
    return 0;
  }
  
  return sercom->readDataUART();
}


void GC_Uart::handle_UART_error( Error *error_p )
{
  sercom->acknowledgeUARTError();
  if (sercom->isFrameErrorUART()) {
    // frame error, next byte is invalid so read and discard it
    sercom->readDataUART();
    if( error_p )
      *error_p = FRAME_ERROR;

    sercom->clearFrameErrorUART();
  }  
  // TODO: if (sercom->isBufferOverflowErrorUART()) ....
  // TODO: if (sercom->isParityErrorUART()) ....
  sercom->clearStatusUART();
}
