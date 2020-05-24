# Gadget Coroutines

_Stacked coroutines for the bare-metal environment._

## What can they do?
 - Run more than one code function at the same time. They _co-operate_ 
   in sharing the machine. When one of them wants to stop, it calls 
   `delay()` or `yield()`. When it's time to continue, `delay()` or 
   `yield()` returns.
 - This function can call other functions, which can call `delay()` or
   `yield()`.
 - Coroutine-local storage is supported via gcc's `__thread`.
 - Coroutines may be invoked as interrupt service routines, and can 
   _hop_ from one interrupt to another. This lats a coroutine respond 
   rapidly to an event.

## What platforms are supported?
 - Only tested on AdaFruit Trinket M0, which is based on Atmel 
   ATSAMD21E18, which is built around the ARM Cortex M0+.
 - Porting to other bare-metal Cortex M0+ systems should be 
   straightforward.

## What examples are there?
 - A simple foreground-only LED flashing example (`flashing.ino`)
 - An LED-flashing example that demonstrates hopping onto a timer 
   interrupt (`hopping.ino`).
 - A DMX receiver (`dimmer.ino`), that demonstrates:
   - Hopping on to level change interrupt for frame pulse detection
   - Hopping on to UART receive interupt to read frame data 
   - DotStar LED
   - SSD1306 OLED display
 - `dimmer.ino` can also run the SSD1306 OLED example concurrent with 
   the DMX receive example - _impossible without stacked coroutines!_.

## More documentation to follow
   
(C) John Graley [LGPL license](license.md) applies.
