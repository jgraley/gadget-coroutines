/**
 * GC_Uart.h 
 * gadget-coroutines
 * Stacked coroutines for the Arduino environment.
 * (C) 2020 John Graley; BSD license applies.
 * 
 * Variaitons on Uart.h for coroutines
 */
#ifndef GC_Uart_h
#define GC_Uart_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include "Coroutine.h"

class GC_Uart : public Uart
{
public:  
  enum Error
  {
      NO_ERROR = 0x0000,
      FRAME_ERROR = 0x0001
  };

  GC_Uart(SERCOM *_s, void (**_vector)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX);
  GC_Uart(SERCOM *_s, void (**_vector)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinRTS, uint8_t _pinCTS);
  
  void begin(unsigned long baudRate);
  void begin(unsigned long baudrate, uint16_t config);
  void end();  
  int read( Error *error_p = nullptr );
  
private:
  void handle_UART_error( Error *error );
  SERCOM *sercom;
  void (**vector)();
};

#endif
