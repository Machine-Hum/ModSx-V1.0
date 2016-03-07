ModSx-V1.0
==========

Please note this project has been discontined, see ModSx-V2.0.

https://github.com/Machine-Hum/ModSx-V2.0


The ModSx isa cross between the Arduino and a modular synths. 
Each module is a PCB that stacks on top of other modules. DC lines and signaling lines flow up 
the stack, these provide power, analog data and digital data to all the connected modules. Right 
angle headers are used for analog signal routing where needed. Wires are used to connect channels. 
The purpose is to reduce cost of modular synths by removing the mechanical aspect. Boards will use 
through hole components where possible, to enable the user to solder their own modules. The entire 
project will increase the accessibility of high quality audio components while increasing the 
customizability, yet keeping the complexity low to the end user.

PowerBoard
--------
The power board, takes DC Vin from a large right angle screw terminal or power jack and conditions it to +3.3V, +5V, +0.5(Vin) and -0.5(Vin). Vin typical would be +24V, giving a +12V and -12V. 

AuxBreak
--------
The Aux breakout, breaks out standard 3.5mm auxiliary cables into pins. The pins serve as interconnects for signal routing.

Proto Board
--------
The protoboard is for prototyping future circuits, basic hole patterns on the top for soldering.

8Bit LFO / VCO
--------
Low frequency oscillator (1-20Hz) sin, triangle or square wave. Takes in 0-10V into input pin, frequency is scaled linearly throughout the range. Waveform is selected from 0/10V discrete signal on pins.
Ideally the same board will be used for 2Khz-20Khz VCO, but I’m worried that the uC will not be able to keep up. VCO might be square wave only.

Power Amp
--------
The power amp is a 4 channel low wattage amplifier based off the LM386. 

-Walker
