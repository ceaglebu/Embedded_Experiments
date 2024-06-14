// ---------------------------- //
// TIMER INTERRUPT BLINK        //
// USE INTERRUPT FOR EFFICIENCY //
// ---------------------------- //

#define __AVR_ATmega328__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define CLOCKRATE 15998796
#define SECONDS 2
#define PRESCALER 10
#define PRESCALED_SECOND (SECONDS * CLOCKRATE >> PRESCALER)


ISR(TIMER1_COMPA_vect) {
    // toggle light
    PORTB ^= _BV(PORTB5);
}

int main(void) {
    // Set pinMode to input
    DDRB |= _BV(PORTB5);

    // Set timer mode -> WGM1 3:0 = 4
    // Set timer clock -> CS1 2:0 = 3b101
    TCCR1B |= _BV(WGM12) | _BV(CS12) | _BV(CS10);

    // Set blink time -> OCR1A = 16b00111101000001000
    OCR1A = PRESCALED_SECOND;

    // Reset timer -> TCNT1 = 0
    TCNT1 = 0x0000;

    // turn on interrupt -> sei(), OCIE1A = 1
    sei();
    TIMSK1 |= _BV(OCIE1A);

    while (true) {
        // Do somethin
    }

}