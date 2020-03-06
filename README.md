# gadget-coroutines
Coroutines for Gadgets

## Enabling exceptions

Find files called platform.txt under ~/.arduino*/packages/*/adafruit/hardware
Change `-fno-exceptions` into `-fexceptions` in either the most specialised one
(i.e. most specific to your platform) or all the ones that relate to your platform.
Suggest taking care if you've got multiple platforms installed and only want to 
change some of them. You'll have to understand the hierarchy and how the overriding 
works.

To get gadget coroutines into your include path, you could try adding 
`"-I{build.core.path}/gadget-coroutines"` to the compiler flags, finding out
where that is on your machine by doing a verbose verify in the IDE and symlinking 
from there to your gadget coroutines git workspace.

## Handy links

 - http://www.arduino.cc/en/Guide/Libraries
 - https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ
