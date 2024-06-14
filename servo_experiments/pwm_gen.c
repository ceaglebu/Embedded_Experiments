// --------------------------------------- //
// GENERATE PWM SIGNAL USING FAST PWM MODE //
// CALIBRATE SERVO CYCLE COUNTS            //
// --------------------------------------- //

#define __AVR_ATmega328P__
#include <avr/io.h>
#include <stdbool.h>
#include <avr/delay.h>
#include <stdbool.h>

// PWM Generator

void write_servo(int servo_pin, float angle) {
    int value = (angle / 180) * 31 + 7;
    OCR0B = value;
    return;
}

int main(void) {

    // DDRB |= _BV(DDB5);
    DDRD |= _BV(DDD5) | _BV(DDD6);

    // Select prescale 1024 clock to get 16 ms periods
    TCCR0B |= _BV(CS02) | _BV(CS00);

    // Set mode to Fast PWM
    TCCR0A |= _BV(WGM00) | _BV(WGM01);

    // Set PWM pulse widths: A = max (not used) B = reasonable servo pulse width
    OCR0A = 8;
    OCR0B = 8;

    // Non - Invert both outputs
    TCCR0A |= _BV(COM0A1) | _BV(COM0B1);

    while (true) {
        write_servo(5, 0);
        _delay_ms(5);
    }
}