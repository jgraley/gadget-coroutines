/**
 * @file Integration.cpp
 * ### `hopping-coroutines`
 * _Stacked coroutines for the Arduino environment._\n
 * @copyright (C) 2020 John Graley; BSD license applies.
 */

#include "Coroutine.h"

#include <cstring>
#include <functional>
#include <csetjmp> 
#include <cstdint>

#include "Arduino.h"

using namespace std;
using namespace HC;
using namespace Arm;

// Implement Arduino yield operation (called by eg delay()) to yield any 
// coroutine that might be running. This makes functions like delay() 
// become seemingly coroutine-aware.
extern  "C" void __attribute__((used)) yield(void) 
{
  Coroutine::yield();
}

// This does what the system yield does if you don't over-ride it.
extern void system_idle_tasks()
{
#if defined(USE_TINYUSB)
  tud_task();
  tud_cdc_write_flush();
#endif
}


void bring_in_Integration()
{
}
