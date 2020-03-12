/*
  Coroutines.cpp - Coroutines for Gadgets.
  Created by John Graley, 2020.
  (C) John Graley LGPL license applies.
*/

#include "Coroutines.h"

#include <cstring>
#include <functional>
#include <csetjmp> 
#include <cstdint>

#include "Arduino.h"
extern  "C" void yield(void)
{
#if defined(USE_TINYUSB)
  tud_task();
  tud_cdc_write_flush();
#endif
  Coroutine::yield(); 
}
