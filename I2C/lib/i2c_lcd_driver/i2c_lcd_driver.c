#include "i2c_lcd_driver.h"
#include "..\i2c_hal\i2c_hal.h"
#include <util/delay.h>
#include <stdbool.h>

#define BT (3)
#define EN (2)
#define RS (1)
#define RW (1 << 1)

volatile bool backlight_on = false;
volatile bool cursor_on = false;
volatile bool blink_on = false;

// Set data bits, turn enable bit on, turn enable bit off
// pinmap = Data bits in upper nibble, BT = 0, E = 0, R/W = R/W, RS = RS
static void send_4_instruction_bits(uint8_t pinmap) {
    pinmap = pinmap | (backlight_on << BT);
    i2c_transmit_bytes(0x27, (uint8_t[]) {pinmap, pinmap | (1 << EN), pinmap}, 3);
}

// Send 8 bit command. Use when you want R/W = 0 and RS = 0
static void send_instruction(uint8_t instruction) {
    send_4_instruction_bits(instruction & 0xF0);
    send_4_instruction_bits((instruction & 0x0F) << 4);
}

static void refresh_cursor() {
    send_instruction(0x0C | (cursor_on << 1) | blink_on);
}

void lcd_init() {
    i2c_init();

    // Get into 4 bit command mode because I2C converter only connects to 4 data bits.
    // Other 4 connections are other LCD Pins.
    // Since the lcd is in 8 bit mode right now, command has to be sent differently
    send_4_instruction_bits(0x20);
    _delay_us(37);

    // Set 2 line mode
    send_instruction(0x28);
    _delay_us(37);

    // Clear display
    lcd_clear();

    // Turn on display
    send_instruction(0x0C);
    _delay_us(37);
}

void lcd_cursor_on() {
    cursor_on = true;
    refresh_cursor();
}

void lcd_cursor_off() {
    cursor_on = false;
    refresh_cursor();
}

void lcd_blink_on() {
    blink_on = true;
    refresh_cursor();
}

void lcd_blink_off() {
    blink_on = false;
    refresh_cursor();
}

void lcd_clear() {
    send_instruction(0x01);
    _delay_us(1600);
}

void lcd_backlight_on() {
    backlight_on = true;
    i2c_transmit_byte(0x27, 1 << BT);
}

void lcd_backlight_off() {
    backlight_on = false;
    i2c_transmit_byte(0x27, 0 << BT);
}

// Writes data to DDRAM. Use when you want R/W = 0 and RS = 1
void lcd_write_letter(char letter) {
    send_4_instruction_bits(letter & 0xF0 | RS);
    send_4_instruction_bits(((letter & 0x0F) << 4) | RS);
}

void lcd_write_string(const char * string) {
    for (size_t idx_letter = 0; string[idx_letter] != '\0'; idx_letter += 1) {
        lcd_write_letter(string[idx_letter]);
        _delay_us(41);
    }
}

// Set cursor position. Row 0 or 1, col 0-15
void lcd_set_cursor_pos(int row, int col) {
    uint8_t cursor_pos = 0x40 * row + col;
    send_instruction(0x80 | cursor_pos);
}

void lcd_clear_letter() {
    lcd_write_letter(0x20);
}

void lcd_set_backwards() {
    send_instruction(0x04);
}

void lcd_set_forwards() {
    send_instruction(0x06);
}

// If backspacing multiple characters, use lcd_set_backwards, lcd_clear_letter, then lcd_set_forwards()
void lcd_backspace() {
    lcd_set_backwards();
    lcd_clear_letter();
    lcd_set_forwards();
}

void lcd_shift_left() {
    send_instruction(0x14);
}

void lcd_shift_right() {
    send_instruction(0x10);
}
