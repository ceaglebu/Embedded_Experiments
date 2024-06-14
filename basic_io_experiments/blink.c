// -------------------------------------- //
//  FIRST PROGRAM IN EMBEDDED C           //
//  BLINK LIGHT BY WRITING TO REGISTERS   //
// -------------------------------------- //

#define __AVR_ATmega328__
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define BLINK_MS 3000

int main() {
    // Write to control register to change pinMode to output
    // Registers = small chunks of really fast memory
    // Compilers handle registers under the hood
    // Registers interact with features of the processor

    // DDRB register controls the pinModes for port B
    // pin 5 of port B = pin 13 on arduino
    // Can find all of this in data sheet
    // Use |= to only affect the one bit
    
    //DDRB |= 0b00100000;
    DDRB |= _BV(PORTB5); // _BV = binary value (macro)
                         // #define _BV(X) = 1 << X
                         // PORTB5 = 5

    while (true) {
        // PORTB |= _BV(PORTB5);
        // _delay_ms(BLINK_MS);
        // PORTB &= ~_BV(PORTB5); // &= 0b11011111, used to set bit 5 to 0
        // _delay_ms(BLINK_MS);

        // Toggle instead of on / off
        PORTB ^= _BV(PORTB5);
        _delay_ms(BLINK_MS);
    }
}