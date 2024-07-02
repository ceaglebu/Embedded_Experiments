#include "i2c_lcd_driver.h"
#include "..\i2c_hal\i2c_hal.h"
#include <util/delay.h>
#include <stdbool.h>

#define BT (3)
#define EN (2)
#define RS (1)
#define RW (1 << 1)

LCD* a_active_lcd = NULL;

volatile bool backlight_on = false;
volatile bool cursor_on = false;
volatile bool blink_on = false;
volatile bool backwards = false;

// Set data bits, turn enable bit on, turn enable bit off
// pinmap = Data bits in upper nibble, BT = 0, E = 0, R/W = R/W, RS = RS
static void send_4_instruction_bits(uint8_t pinmap) {
    pinmap = pinmap | ((a_active_lcd->backlight_on) << BT);
    i2c_transmit_bytes(a_active_lcd->address, (uint8_t[]) {pinmap, pinmap | (1 << EN), pinmap}, 3);
}

// Send 8 bit command. Use when you want R/W = 0 and RS = 0
static void send_instruction(uint8_t instruction) {
    send_4_instruction_bits(instruction & 0xF0);
    send_4_instruction_bits((instruction & 0x0F) << 4);
}

static void refresh_cursor() {
    send_instruction(0x0C | ((a_active_lcd->cursor_on) << 1) | (a_active_lcd->blink_on));
    _delay_us(37);
}


void lcd_create(LCD* a_lcd, uint8_t address) {
    *a_lcd = (LCD) {
        .address = address,
        .backlight_on = false,
        .cursor_on = false,
        .blink_on = false,
        .backwards = false
    };
}

void lcd_select(LCD* a_lcd) {
    a_active_lcd = a_lcd;
}

lcd_boot(LCD* a_lcd, uint8_t address) {
    lcd_create(a_lcd, address);
    lcd_select(a_lcd);

    lcd_init();
    lcd_backlight(LCD_ON);
}

void lcd_init() {
    i2c_init();

    // Need to do funky stuff so power on works with reset button, not just 
    // when plugged in
    _delay_ms(15);
    send_4_instruction_bits(0x30);
    _delay_ms(4.1);
    send_4_instruction_bits(0x30);
    _delay_us(100);
    send_4_instruction_bits(0x30);

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

void lcd_cursor(bool on) {
    (a_active_lcd->cursor_on) = on;
    refresh_cursor();
}

void lcd_blink(bool on) {
    (a_active_lcd->blink_on) = on;
    refresh_cursor();
}

void lcd_backlight(bool on) {
    (a_active_lcd->backlight_on) = on;
    i2c_transmit_byte(0x27, on << BT);
}

void lcd_clear() {
    (a_active_lcd->backwards) = false;
    send_instruction(0x01);
    _delay_us(1600);
}

// Writes data to DDRAM. Use when you want R/W = 0 and RS = 1
void lcd_write_letter(char letter) {
    send_4_instruction_bits(letter & 0xF0 | RS);
    send_4_instruction_bits(((letter & 0x0F) << 4) | RS);
    _delay_us(41);
}

void lcd_write_string(const char * string) {
    for (size_t idx_letter = 0; string[idx_letter] != '\0'; idx_letter += 1) {
        lcd_write_letter(string[idx_letter]);
    }
}

void lcd_write_blank_letter() {
    lcd_write_letter(0x20);
}

// Set cursor position. Row 0 or 1, col 0-15
void lcd_set_cursor_pos(int row, int col) {
    uint8_t cursor_pos = 0x40 * row + col;
    send_instruction(0x80 | cursor_pos);
    _delay_us(37);
}

void lcd_delete_letter() {
    lcd_write_blank_letter();
    lcd_shift_cursor((a_active_lcd->backwards));
}

void lcd_backspace() {
    lcd_shift_cursor((a_active_lcd->backwards));
    lcd_write_blank_letter();
    lcd_shift_cursor((a_active_lcd->backwards));
}

void lcd_backspace_many(int count) {
    lcd_shift_cursor((a_active_lcd->backwards));
    lcd_set_direction((a_active_lcd->backwards));
    for (int cnt_backspaced = 0; cnt_backspaced < count; cnt_backspaced += 1) {
        lcd_write_blank_letter();
    }
    lcd_shift_cursor((a_active_lcd->backwards));
    lcd_set_direction((a_active_lcd->backwards));
}

// lcd_set_direction(lcd.backwards) will always flip direction
void lcd_set_direction(bool right) {
    (a_active_lcd->backwards) = !right;
    send_instruction(0x04 | (right << 1));
    _delay_us(37);
}

void lcd_shift(bool right) {
    send_instruction(0x18 | (right << 2));
    _delay_us(37);
}

void lcd_shift_cursor(bool right) {
    send_instruction(0x10 | (right << 2));
    _delay_us(37);
}