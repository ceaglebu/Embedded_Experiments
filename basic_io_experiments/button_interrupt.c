// ---------------------------------------- //
// UTILIZE GPIO INTERRUPTS TO TURN ON LIGHT //
// ---------------------------------------- //

#define __AVR_ATmega328__

#include<avr/io.h>
#include<avr/interrupt.h>
#include<stdbool.h>

ISR(PCINT2_vect) {
    if (PIND & _BV(PIND2)) {
        PORTB |= _BV(PORTB5);
    } else {
        PORTB &= ~_BV(PORTB5);
    }
}

int main(void) {
    // Set up input / output pins
    DDRD &= ~_BV(DDD2);
    DDRB |= _BV(PORTB5);

    // enable interrupts
    sei();

    // request pin interrupt 2
    PCMSK2 |= _BV(PCINT18);
    PCICR |= _BV(PCIE2);

    while (true) {
        // relax man!
    }
}