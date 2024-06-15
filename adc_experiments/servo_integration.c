// Integrates servos with ADC
// Reads ADC input from potentiometers
// Moves servos according to potentiometer position
// Also first time working with multiple adc inputs in the same program (switching input)

#define __AVR_ATmega328P__
#include "adc_hal.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>

typedef struct _Servo {
    volatile uint8_t * pulse_width_cycles;
    int angle;
    int dir;
} Servo;

void Servo_write(Servo* servo, int angle) {
    if (angle == servo->angle) {
        return;
    }

    int pulse_width_cycles = ((float) angle / 180) * 31 + 7;
    *(servo->pulse_width_cycles) = pulse_width_cycles;
    servo->angle = angle;
}

int adc_to_angle(adc_result adc) {
    return (int) (180 * (float) adc / 1023);
}

int main(void) {
    // Initialize potentiometers
    adc_init();
    adc_enable_pin(ADC_1);
    adc_enable_pin(ADC_2); 

    // Prepare servos
    // D5/6 - PWM Servo outputs (Timer0 fastPWM outputs)
    DDRD |= _BV(DDD5) | _BV(DDD6);

    // Set up fast PWM on timer 0
    // --------------------------

    // Select clock
    TCCR0B |= _BV(CS02) | _BV(CS00);

    // Select PWM mode and non-invert outputs
    TCCR0A |= _BV(WGM01) | _BV(WGM00);
    TCCR0A |= _BV(COM0A1) | _BV(COM0B1);

    Servo servo_1 = (Servo) {.pulse_width_cycles = &OCR0B, .angle = 0, .dir = 1};
    Servo servo_2 = (Servo) {.pulse_width_cycles = &OCR0A, .angle = 0, .dir = 1};
    Servo* a_servo_1 = &servo_1;
    Servo* a_servo_2 = &servo_2;

    // Enable interrupts
    sei();

    adc_result pot_1;
    adc_result pot_2;

    int angle_1;
    int angle_2;

    while (true) {
        // Read ADCs
        adc_select_pin(ADC_1);
        pot_1 = adc_read();

        adc_select_pin(ADC_2);
        pot_2 = adc_read();

        // Move servos
        Servo_write(a_servo_1, adc_to_angle(pot_1));
        Servo_write(a_servo_2, adc_to_angle(pot_2));
        _delay_ms(5);
    }
}