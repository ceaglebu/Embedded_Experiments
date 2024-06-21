#include <avr/io.h>
#include <serial_hal.h>
#include <util/delay.h>

int main (void) {
    
    serial_begin();

    DDRB |= _BV(PORTB5);
    PORTB &= ~_BV(PORTB5);
    

    while (1) {
        _delay_ms(1000);
        PORTB &= ~_BV(PORTB5);
        _delay_ms(1000);
        serial_send_string("Hello World\n");
        PORTB |= _BV(PORTB5);
    }
}