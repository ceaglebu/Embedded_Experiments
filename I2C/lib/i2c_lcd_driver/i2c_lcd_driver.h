#ifndef __I2C_LCD_DRIVER_H__
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

void lcd_init();

void lcd_cursor_on();
void lcd_cursor_off();

void lcd_blink_on();
void lcd_blink_off();

void lcd_clear();

void lcd_backlight_on();
void lcd_backlight_off();

void lcd_write_letter(char letter);
void lcd_write_string(const char* string);

void lcd_set_cursor_pos(int row, int col);

void lcd_backspace();
void lcd_set_backwards();
void lcd_set_forwards();
void lcd_clear_letter();

void lcd_shift_left();
void lcd_shift_right();

#endif