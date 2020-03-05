# gadget-coroutines
Coroutines for Gadgets

## Enabling exceptions

Find files called platform.txt under ~/.arduino*/packages/*/adafruit/hardware
Change `-fno-exceptions` into `-fexceptions` in either the most specialised one
(i.e. most specific to your platform) or all the ones that relate to your platform.
Suggest taking care if you've got multiple platforms installed and only want to 
change some of them. You'll have to understand the hierarchy and how the overriding 
works.

## Handy links

 - http://www.arduino.cc/en/Guide/Libraries
 - https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ
