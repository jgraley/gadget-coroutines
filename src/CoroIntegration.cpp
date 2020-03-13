/**
 * CoroIntegration.cpp - Coroutines for Gadgets.
 * Created by John Graley, 2020.
 * (C) John Graley LGPL license applies.
 * 
 * Here, we integrate Gadget Coroutines into the Arduino environment.
 */

#include "Coroutines.h"

#include <cstring>
#include <functional>
#include <csetjmp> 
#include <cstdint>

#include "Arduino.h"

// This makes sure the CLS is a NULL pointer for the fore-ground
// context (i.e. when outside any coroutine)
// CLS is Coroutine-Local Storage
void __attribute__ ((constructor)) init_baseline_cls()
{
    set_cls( nullptr );
}


// Implement Arduino yield operation (called by eg delay()) to yield any 
// coroutine that might be running. This makes functions like delay() 
// become seemingly coroutine-aware.
extern  "C" void yield(void)
{
  // Try not to lose TinyUSB polling
#if defined(USE_TINYUSB)
  tud_task();
  tud_cdc_write_flush();
#endif

  Coroutine::yield(); 
}