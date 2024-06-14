# Basic IO Experiments
The very first initial programs written in embedded C to get a handle on embedded development.

## Blink
Blinks the arduino on-board light by delaying x ms and toggling bit. Learned how to use the data direction registers and output to GPIO pins. 

## Blink Timer
Blinks the arduino on-board light by using ATmega328P's timer registers and interrupt service routines. Uses less power than blink by not executing nop instructions.

## Blink Timer Power Save
Blinks the arduino on-board light by using ATmega328P's timer 2 registers and interrupts. Puts processor in sleep mode while light is off.

## Button Light
Learned how to take input from GPIO pins. On-board light turns on when button is held.

## Button Interrupt
Learned how to use ATMega328P's PC interrupts. Triggers interrupt using GPIO port.