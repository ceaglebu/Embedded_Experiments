// ADC Test - Learning how to use ADC to read light level

#define __AVR_ATmega328P__
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

int main(void) {

    // Set up ADC read

    // Set reference voltage
    ADMUX |= _BV(REFS0); // REFS1..0 = 01 -> AVcc with external cap at AREF pin
    
    // Enable ADC
    ADCSRA |= _BV(ADEN) | _BV(ADIE);

    // Disable Digital Input to A0
    DIDR0 |= _BV(ADC0D);
    
    // Select ADC 0
    ADMUX &= ~(0b1111 << MUX0);
 
    // Select ADC input clock
    ADCSRA |= 0b111 << ADPS0;

    volatile uint16_t light_level = ADC;

    // Set up on board light for ouput
    DDRB |= _BV(PORTB5);

    while (true) {
        // Start conversion
        ADCSRA |= _BV(ADSC);
        // Read ADC
        while (ADCSRA & _BV(ADSC));
        
        light_level = ADC;

        if (light_level > 500) {
            PORTB |= _BV(PORTB5);
        } else {
            PORTB &= ~_BV(PORTB5);

        }
        _delay_ms(5);
    }       
}
