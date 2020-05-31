/**
 * @file HC_Uart.cpp
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 */

#include "HC_Uart.h"

#include <functional>
#include <atomic>

using namespace std;
using namespace HC;

HC::Uart::Uart(SERCOM *_s, void (**_vector_p)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX) :
  ::Uart( _s, _pinRX, _pinTX, _padRX, _padTX ),
  sercom( _s ),
  vector_p( _vector_p )
{
}
 
  
HC::Uart::Uart(SERCOM *_s, void (**_vector_p)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinRTS, uint8_t _pinCTS) :
  ::Uart( _s, _pinRX, _pinTX, _padRX, _padTX, _pinRTS, _pinCTS ),
  sercom( _s ),
  vector_p( _vector_p )
{
}


void HC::Uart::begin(unsigned long baudRate)
{
  if( vector_p )
    *vector_p = *me();
  ::Uart::begin(baudRate);
}


void HC::Uart::begin(unsigned long baudrate, uint16_t config)
{
  if( vector_p )
    *vector_p = *me();
  ::Uart::begin(baudrate, config);
}


void HC::Uart::end()
{
  ::Uart::end();
  if( vector_p )
    *vector_p = nullptr;
}


int HC::Uart::read( Error *error_p )
{
  if( error_p )
    *error_p = NO_ERROR;
  wait( [=]{ return sercom->isUARTError() || sercom->availableDataUART(); } );

  if(sercom->isUARTError())
  {
    handle_UART_error(error_p);  
    return 0;
  }
  
  return sercom->readDataUART();
}


void HC::Uart::handle_UART_error( Error *error_p )
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
