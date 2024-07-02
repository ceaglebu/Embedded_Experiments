#ifndef __I2C_LCD_DRIVER_H__
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <stdbool.h>
#include <stdint.h>

// Driver for LCD I2C screen with address 0x27
// TODO: make compatible with multiple screens on bus.
//          -- don't hardcode addresss, each function is passed LCD object
// fix backspace: use cursor shifting

#define LCD_LEFT (false)
#define LCD_RIGHT (true)
#define LCD_ON (true)
#define LCD_OFF (false)

typedef struct {
    uint8_t address;
    bool backlight_on;
    bool cursor_on;
    bool blink_on;
    bool backwards;
} LCD;

void lcd_create(LCD* a_lcd, uint8_t address);
void lcd_select(LCD* a_lcd);
void lcd_init();

// Avoid boilerplate if you want to create, initialize, turn on, and use a new LCD
void lcd_boot(LCD* a_lcd, uint8_t address);

void lcd_cursor(bool on);
void lcd_blink(bool on);
void lcd_backlight(bool on);

void lcd_clear();
void lcd_write_letter(char letter);
void lcd_write_string(const char* string);
void lcd_write_blank_letter();
void lcd_set_cursor_pos(int row, int col);

void lcd_delete_letter();
void lcd_backspace();
void lcd_backspace_many(int count);

void lcd_set_direction(bool right);
void lcd_shift(bool right);
void lcd_shift_cursor(bool right);

#endif