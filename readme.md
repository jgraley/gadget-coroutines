# Gadget Coroutines

_Stacked coroutines for the bare-metal environment._

## What can they do?
 - Run many functions at the same time. These _co-operate_ to share the 
   CPU. When one of them wants to pause processing, it calls 
   `delay()` or `yield()`. When it's time to continue, `delay()` or 
   `yield()` will return.
 - **Stacked**: a coroutine can call ordinary functions, to any depth, 
   which can call `delay()` or `yield()`.
 - **TLS**: coroutine-local storage is supported via gcc's `__thread`. 
 - **Hopping**: coroutines may be invoked from interrupt service 
   routines. This enables a coroutine to respond rapidly to events.

## What platforms are supported?
 - Only tested on **AdaFruit Trinket M0**, which is based on **Atmel 
   ATSAMD21E18**, which is built around the **ARM Cortex M0 plus**.
 - Porting to other bare-metal Cortex M systems should be 
   straightforward.

## What examples are there?
 - A simple foreground-only LED flashing example (`flashing.ino`)
 - An LED-flashing example that demonstrates hopping onto a timer 
   interrupt (`hopping.ino`).
 - A DMX receiver (`dimmer.ino`), that demonstrates:
   - Hopping on to level-change interrupt for frame pulse detection
   - Hopping on to UART receive interupt to read frame data 
   - DotStar LED
   - SSD1306 OLED display
 - `dimmer.ino` can also run the SSD1306 OLED display driver example 
   concurrent with the DMX receiver - _impossible without
   coroutines!_.

## More documentation to follow
   
(C) John Graley [LGPL license](license.md) applies.
