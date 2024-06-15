# ADC Experiments
Experiments with the onboard analog to digital converters of the ATmega328P.

# adc_read.c
First interaction with ADC.

# adc_hal.c / adc_hal.h / adc_hal_test.c
Mini library to make interactions with ADC easier. adc_hal_test tests the functionality of the library.

# servo_integration.c
Integrates servos with ADC
Reads ADC input from potentiometers
Moves servos according to potentiometer position
Also first time working with multiple adc inputs in the same program (switching input)