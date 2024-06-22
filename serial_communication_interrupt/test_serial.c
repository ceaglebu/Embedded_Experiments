#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif
#include "lib\serial_hal\serial_hal.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

int main(void) {
    serial_begin();

    DDRB |= _BV(DDB5);

    sei();

    while(1) {
        serial_send_string("Hello embedded\n");
        PORTB ^= _BV(PORTB5);
        _delay_ms(500);
    }

}