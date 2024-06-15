// Identical behavior to adc_read.c. Uses adc_hal header file instead, which uses interrupt instead of checking ADSC bit directly.

#define __AVR_ATmega328P__
#include "adc_hal.h"
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/delay.h>
#include <assert.h>

int main(void) {
    sei();

    adc_init();
    ADC_selection adc_selection = ADC_1;
    adc_enable_pin(adc_selection);
    adc_select_pin(adc_selection);
    assert(adc_get_active_pin() == adc_selection);

    adc_result voltage;

    DDRB |= _BV(PORTB5);

    while (true) {
        voltage = adc_read();
        if (voltage > 500) {
            PORTB |= _BV(PORTB5);
        } else {
            PORTB &= ~_BV(PORTB5);
        }
        _delay_ms(5);
    }
}