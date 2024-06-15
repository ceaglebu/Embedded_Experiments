// ADC Hardware abstraction layer - reusable mini library to get ADCs working
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

typedef uint16_t adc_result;

typedef enum {
    ADC_0,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5
} ADC_selection;


// enables adc, selects voltage reference, and selects input clock. Also enables ADC complete interrupt.
void adc_init(void);

// turns off digital input. Not necessary for adc functions, but increases power efficiency
void adc_enable_pin(ADC_selection choice);

// Re-enables digital input
void adc_disable_pin(ADC_selection choice);

// Changes ADC mux
void adc_select_pin(ADC_selection);

// Read adc value of selected pin
adc_result adc_read(void);

ADC_selection adc_get_active_pin(void);