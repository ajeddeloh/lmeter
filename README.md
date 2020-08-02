STM32L476G-Discovery Board Inductance Meter
---------

This is a project to measure inductors in the uH-mH range using an STM32L476G-Discovery
board. It uses the DAC to generate a sine wave which is applied to a resistor and the
unknown inductor in series (see the [schematic](hardware/schematic.pdf)). 
The voltage is measured across the entire circuit as well as the resistor. The complex
ratio of these voltages is then used to calculate the inductance of the inductor.

There are definitely better ways to measure inductance (such as measuring the resonant
frequency of the inductor and a known capacitor), but I wanted to experiment with doing
it on a microcontroller using the DAC and ADC.

### Theory

The inductor and resistor in series act as a voltage divider. The voltage across the inductor
(Vi) in the divider is given by

`Vi = (Vt*Z) / (R+Z)`

Where:
 - Z is the impedance of the inductor
 - R is the resistance of the resistor
 - Vt is the voltage across both in series

Rearranging, we find that:

`Z = R(Vi/Vt)/(1-Vi/Vt)`

Since the impedance of an inductor is `i*2*pi*f*L`, we can get the inductance from the
impedance if we know the frequency.

The DAC generates a sine wave at a known frequency and the ADC measures the voltages over time.

Trying to find the amplitudes and phases of the waveforms in the time domain would be hard and
noise would be hard to account for, so instead we use the discrete fourier transform to extract
the amplitudes at exactly the frequency we applied.

Since we only care about once frequency, we don't need to compute the whole transform; we only
need to evaluate it at the frequency of the sine wave we applied. This runs in O(1) space and
O(n) time, meaning we can calculate it on the fly for arbitrarily long captures.

Using Fourier analysis helps reduce noise by filtering out all noise except that at the
frequency of interest, thus improving SNR.

Normally windowing functions would be needed, since the DFT assumes the data is cyclic.
However, in our case we can insure this assumption is always true by ensuring we always end
data collection after an integer number of cycles. Furthermore, the ADC and DAC share the same
clock, so we don't need to worry about drift between them. Finally, since we use the ratio of
the results of two DFTs at the same frequency, if there were any windowing affects it would
affect both equally and be cancelled out.

### Op amp selection

The op amps are used exclusively as buffers and as a virtual ground.
I made this project with an LM324 op amp because it was the only op amp I had on hand that
runs on 5V. It is *barely* adequate. It has two very undesirable characteristics:
 - Bad crossover distortion: the schematic shows each output has a resistor to ground to
 prevent it from switching between sourcing and sinking current. W2AEW has a 
 [great video](https://www.youtube.com/watch?v=VgodYtiD_F0) on this.
 - Low slew rate (0.5V/us): This limits the amplitude of the sine wave that can be used at
 high frequency.

A much better selection would be the MCP6294 which is pin compatible, though I haven't tested
with it.

### Changes to the STM32L476G-Discovery board:

A few changes are need to be made to the board:

 - The 8MHz clock from the STLink controller needs to be connected to the HSE clock input
 of the microcontroller so we can run at 80MHz. Open solder bridge 22 and close solder
 bridge 18.
 - The PA0-5 pins are wired to the debouncing connections for the joystick. These interfere
 with our use. Remove R52, R54, R55, R56, R58, and C43 to disconnect these pins from the
 joystick buttons.

### Project Layout

 - STM32CubeL4: git submodule of the upstream STM32CubeL4 repo. In general if
 things dont need modification I dont make copies in src. This should make it
 easier to update if there's an update to the cube stuff.
 - src: source code
 - hal: the HAL configuration file must be in a directory that is part of the 
 include path, so it's in its own directory so that directory can be passed to
 the compiler with -I
 - hardware: kicad files and a pdf of the schematic
