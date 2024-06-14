// ----------------------------------------------- //
// USE GPIO TO TURN ON LIGHT WHEN BUTTON PRESSED   //
// ----------------------------------------------- //

#define __AVR_ATmega328__

#include<avr/io.h>
#include<stdbool.h>

int main(void) {
    // Set pinMode to input for digital pin 2 (Port D2)
    DDRD &= ~_BV(DDD2);

    // Set pinMode to output for digital pin 13 (Port B5)
    DDRB |= _BV(DDD5);

    while(true) {
        // if digital pin 2 is high, set digital pin 13 high.
        // otherwise, set digital pin 2 low
        if ((PIND & _BV(PIND2)) ^ ((PINB & _BV(PINB5)) >> 3)) {
            PORTB ^= _BV(PORTB5);
        }
    }
}
