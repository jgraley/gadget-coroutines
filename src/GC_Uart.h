/**
 * @file GC_Uart.h 
 * ### `gadget-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 * 
 * @brief Uart class tailored for coroutines
 */
#ifndef GC_Uart_h
#define GC_Uart_h

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include "Coroutine.h"

namespace GC
{

/**
 * @brief Coroutine Uart class.
 * 
 * A variation of the `::Uart` class customised for use in coroutines.
 * 
 * Buffering is bypassed. Read and write operations block the caller 
 * until a character is available (calling `yield()` while they wait). 
 * UART receive errors can be detected and returned. The constructor
 * can be given a pointer to a RAM interrupt vector.
 */
class Uart : public ::Uart
{
public:  
  /**
   * UART error codes bitfield
   */
  enum Error
  {
      NO_ERROR = 0x0000,
      FRAME_ERROR = 0x0001
  };

  /**
   * Similar to `::Uart` constructor, with one extra parameter. 
   *  
   * @param _vector_p pointer to an interrupt vector.
   */ 
  Uart(SERCOM *_s, void (**_vector_p)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX);

  /**
   * Similar to `::Uart` constructor, with one extra parameter. 
   *  
   * @param _vector_p pointer to an interrupt vector.
   */ 
  Uart(SERCOM *_s, void (**_vector_p)(), uint8_t _pinRX, uint8_t _pinTX, SercomRXPad _padRX, SercomUartTXPad _padTX, uint8_t _pinRTS, uint8_t _pinCTS);
  
  /**
   * Similar to `::Uart::begin()`. 
   * 
   * Starts up the serial port. If an interrupt vector was supplied to the
   * constructor, it will be updated here to point to our ISR. 
   */ 
  void begin(unsigned long baudRate);

  /**
   * Similar to `::Uart::begin()`. 
   * 
   * Starts up the serial port. If an interrupt vector was supplied to the
   * constructor, it will be updated here to point to our ISR. 
   */ 
  void begin(unsigned long baudrate, uint16_t config);

  /**
   * Similar to `::Uart::end()`. 
   * 
   * Shuts down the serial port. If an interrupt vector was supplied to the
   * constructor, it will be reset to `NULL` here. 
   */ 
  void end();  


  /**
   * Similar to `::Uart::read()`, with one extra parameter. 
   * 
   * Blocks while reading a character from the serial port.
   * 
   * @param error_p if non-`NULL` the location pointed to is updated with an error code.
   */ 
  int read( Error *error_p = nullptr );
  
private:
  void handle_UART_error( Error *error );
  SERCOM *sercom;
  void (**vector)();
};

} // namespace

#endif
