STM32L476G-Discovery Board Inductance Meter
---------

WIP WIP WIP

A simple, low frequency inductance meter for high value inductors

There are definitely better ways to measure inductance, but I wanted to
experiment with doing it on a microcontroller using the ADC.

### Project Layout

 - STM32CubeL4: git submodule of the upstream STM32CubeL4 repo. In general if
 things dont need modification I dont make copies in src. This should make it
 easier to update if there's an update to the cube stuff.
 - src: source code
 - hal: the HAL configuration file. must be in a directory that is part of the 
 include path, so it's in its own directory so that directory can be passed to
 the compiler with -I
