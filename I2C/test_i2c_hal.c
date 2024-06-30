#define __AVR_ATmega328P__
#include <avr/interrupt.h>
#include "lib\i2c_hal\i2c_hal.h"

void test() {
    i2c_init();
    i2c_transmit_byte(0x27, 0xFF);
    i2c_transmit_bytes(0x27, (uint8_t[]) {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08}, 8);
}

int main(void) {
    sei();

    test(); 

    while (1) {
    }
}