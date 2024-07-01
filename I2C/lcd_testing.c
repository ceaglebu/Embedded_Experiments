#define __AVR_ATmega328P__
#include "lib\i2c_hal\i2c_hal.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#define BT (1 << 3)
#define EN (1 << 2)
#define RS (1)
#define RW (1 << 1)

volatile bool backlight_on = false;

// Set data bits, turn enable bit on, turn enable bit off
// pinmap = Data bits in upper nibble, BT = 0, E = 0, R/W = R/W, RS = RS
void send_4_instruction_bits(uint8_t pinmap) {
    pinmap = pinmap | (backlight_on << 3);
    i2c_transmit_bytes(0x27, (uint8_t[]) {pinmap, pinmap | EN, pinmap}, 3);
}

void send_instruction(uint8_t instruction) {
    send_4_instruction_bits(instruction & 0xF0);
    send_4_instruction_bits((instruction & 0x0F) << 4);
}

void write_letter(uint8_t letter) {
    send_4_instruction_bits(letter & 0xF0 | RS);
    send_4_instruction_bits(((letter & 0x0F) << 4) | RS);
}

void write_string(const char * string) {
    for (size_t idx_letter = 0; string[idx_letter] != '\0'; idx_letter += 1) {
        write_letter(string[idx_letter]);
        _delay_us(41);
    }
}

int main(void) {

    sei();

    i2c_init();
    
    // Initialize LCD
    // I2C -> Pins:
    // D7  D6  D5  D4  BT  E  R/W  RS

    // Get into 4 bit command mode
    // 0   0   1   0   0   X   0   0
    send_4_instruction_bits(0x20);
    _delay_us(37);
    
    // Set 2 lines
    // 0   0   1   0   0   X   0   0
    // 1   0   0   0   0   X   0   0
    send_instruction(0x28);
    _delay_us(37);

    // Clear Display
    // 0   0   0   0   0   X   0   0
    // 0   0   0   1   0   X   0   0
    send_instruction(0x01);
    _delay_us(1600);

    // Turn display on - C = cursor, B = blink
    // 0   0   0   0   1   X   0   0
    // 1   1   C   B   1   X   0   0 
    backlight_on = true;
    send_instruction(0x0F);
    _delay_us(37);

    // Write Character
    // 0   1   0   0   1   X   0   1
    // 1   0   0   0   1   X   0   1
   write_letter('H'); 
   _delay_us(41);

   write_string("Hello World");

    while (1) {
        
    }
}