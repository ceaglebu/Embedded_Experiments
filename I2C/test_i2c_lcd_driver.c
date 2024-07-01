#define __AVR_ATmega328P__
#include "lib\i2c_lcd_driver\i2c_lcd_driver.h"
#include <avr/interrupt.h>
#include <util/delay.h>

int main (void) {
    sei();

    lcd_init();
    lcd_backlight_on();

    lcd_set_cursor_pos(0, 1);

    char letter = 'A';

    while (1) {
        lcd_write_letter(letter++);
        _delay_ms(500);
        lcd_backspace();
        _delay_ms(500);
    }
}