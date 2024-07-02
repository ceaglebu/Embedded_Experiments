#define __AVR_ATmega328P__
#include "lib\i2c_lcd_driver\i2c_lcd_driver.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main (void) {
    sei();
    LCD lcd;
    lcd_boot(&lcd, 0x27);

    // Test cursor
    lcd_write_string("Cursor On:");
    lcd_cursor(LCD_ON);
    _delay_ms(2000);
    lcd_clear();
    lcd_write_string("Cursor Off:");
    lcd_cursor(LCD_OFF);
    _delay_ms(2000);
    lcd_clear();

    // Test blink
    lcd_write_string("Blink On:");
    lcd_blink(LCD_ON);
    _delay_ms(2000);
    lcd_clear();
    lcd_write_string("Blink Off:");
    lcd_blink(LCD_OFF);
    _delay_ms(2000);
    lcd_clear();

    // Test Turning on / off backlight
    lcd_write_string("Turning off");
    lcd_set_cursor_pos(1, 0);
    lcd_write_string("Backlight: 3");
    _delay_ms(1000);
    lcd_backspace();
    lcd_write_letter('2');
    _delay_ms(1000);
    lcd_backspace();
    lcd_write_letter('1');
    _delay_ms(1000);
    lcd_clear();
    lcd_backlight(LCD_OFF);
    _delay_ms(2000);
    lcd_backlight(LCD_ON);
    lcd_write_string("We back");
    _delay_ms(1000);
    lcd_clear();

    // Test writing blank letters
    lcd_write_string("2 blank letters:");
    lcd_set_cursor_pos(1, 0);
    lcd_write_blank_letter();
    lcd_write_blank_letter();
    lcd_write_string("<-");
    _delay_ms(2000);
    lcd_clear();

    // Test setting cursor position
    lcd_write_string("Test set cursor");
    lcd_set_cursor_pos(1, 0);
    lcd_write_string("position");
    lcd_set_cursor_pos(1, 15);
    lcd_write_letter('!');
    _delay_ms(2000);
    lcd_clear();

    // Test backspacing
    lcd_write_string("Backspace this");
    _delay_ms(2000);
    lcd_backspace_many(14);
    lcd_write_string("This is left");
    _delay_ms(1000);
    lcd_clear();

    // Test writing backwards
    lcd_set_direction(LCD_LEFT);
    lcd_set_cursor_pos(0, 15);
    lcd_write_string("Backwards");
    _delay_ms(2000);
    lcd_clear();

    // Test shifting display
    lcd_write_string("Shift Right");
    for (int shift_cnt = 0; shift_cnt < 5; shift_cnt += 1) {
        _delay_ms(500);
        lcd_shift(LCD_RIGHT);
    }
    _delay_ms(500);
    lcd_clear();

    lcd_set_cursor_pos(0, 6);
    lcd_write_string("Shift Left");
    for (int shift_cnt = 0; shift_cnt < 5; shift_cnt += 1) {
        _delay_ms(500);
        lcd_shift(LCD_LEFT);
    }
    _delay_ms(500);
    lcd_clear();

    // Test shifting cursor
    lcd_cursor(LCD_ON);
    lcd_write_string("Cursor Dance");
    for (uint8_t shift_cnt = 0; shift_cnt < 5; shift_cnt += 1) {
        _delay_ms(500);
        lcd_shift_cursor(shift_cnt & 0x01);
    }
    _delay_ms(500);
    lcd_cursor(LCD_OFF);
    lcd_clear();
    
    // Tests complete
    lcd_write_string("Test Complete");
    _delay_ms(2000);
    lcd_clear();
    lcd_backlight(LCD_OFF);

    while (1) {

    }
}