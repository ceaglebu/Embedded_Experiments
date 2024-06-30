# Embedded Software Experiments
Programs created while experimenting and learning embedded software. Written in C for AVR ATmega328P microcontroller on Arduino Uno development board and STM32F446re on STNucleo development board. 

## Highlights
- ### Analog to Digital Conversion
    - Created ADC hardware abstraction layer (HAL)
        - Reads values from selected ADC on ATmega328P
    - Designed simple voltage divider circuits with LDRs and rotary potentiometers to manipulate ADC readings
        - Turned on board arduino light if LDR reaches a certain light level (adc_experiments / adc_read.c and adc_experiments / adc_hal_test.c)
        - Controlled servo position based on ADC readings from rotary potentiometers or LDRs. (adc_experiments/servo_integration.c)

- ### Servo Control
    - Generated PWM signals to control servo position
        - servo_experiments/pwm_gen.c
    - Implemented state machines to create simple systems
        - 2 sweeping servos, only one active at a time. Press a button to switch between them. Press another button to turn on / off. (servo_experiments/two_servos_state_machine.c) 
        - 2 servos controlled by ADC reading from potentiometer voltage divider. Press a button to save current position. Press another button to switch between saved position and potentiometer position. (adc_experiments/save_state.c)

- ### UART Communication
    - Created interrupt-based UART HAL (serial_communication_interrupt/lib/serial_hal)
        - Sends and receive messages over UART
        - Uses a ring buffer to store tranmission data (serial_communication_interrupt/lib/ring_buffer)
        - Used for simple test program that would reply with the same message that was sent from serial monitor

- ### I2C Communication
    - Used polling and interrupt based methods to send messages over I2C (I2C/interrupt_i2c_transmit.c, I2C/polling_i2c_transmit.c)
        - Used Analog Discover 3 logic analyzer for debugging / analysis of working drivers
    - Created interrupt-based I2C HAL
    - Created I2C-LCD driver using my I2C HAL
        - Control of backlight, cursor position, and character at each position
        - Higher-level functions that handle backlight, cursor, and characters to print messages to screen