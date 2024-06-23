#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif
#include "lib\serial_hal\serial_hal.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t attach = 0;

int main(void) {
    serial_begin();

    DDRB |= _BV(DDB5);

    sei();


    while(1) {
        serial_send_char(serial_read_char_sleep(0b011));
        PORTB ^= _BV(PORTB5);
    }

}