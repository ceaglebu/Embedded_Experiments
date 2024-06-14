# Servo Experiments
Experiments involving PWM outputs to work with servos

## Two Servos State Machine
Exploration in the state machine design pattern. Implemented using a two dimensional array of function pointers. Functions are passed "FSM" and "Condition" structs by address. Small use of dynamic memory allocation. Expands on two_servos.c by adding an off state and a power button. Uses polling and delay to capture debounced button inputs. Using interrupts to capture input and power saving mode when in the off state may be more optimal. State machine loosely based on the lookup table implementation in this article: [aticleworld.com](https://aticleworld.com/state-machine-using-c/)

## Two Servos
Experimentation with servo movement rather than putting them in static positions. Accepts button input to switch between two servos in order to verify correct usage of the two outputs from timer 0's fast PWM mode. The active servo will sweep from 0 to 180 degrees.

## pwm_gen
First program I wrote to generate a PWM signal. Uses ATMega328P's Fast PWM mode on timer 0.