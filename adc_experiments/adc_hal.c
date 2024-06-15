// ADC Hardware abstraction layer - reusable mini library to get ADCs working
#include "adc_hal.h"

// STEPS TO BE TAKEN (from adc_read.c)
// ------------------------
// Select Voltage Reference
// Enable ADC
// Select input clock
//
// Disable digital input
// Select ADC
//
// Read conversion

volatile int conversion_done = 1;

ISR(ADC_vect) {
    conversion_done = 1;
}

// adc_init
// - enables adc, selects voltage reference, and selects input clock. Also enables ADC complete interrupt.
void adc_init(void) {
    // Set reference voltage
    ADMUX |= _BV(REFS0); // REFS1..0 = 01 -> AVcc with external cap at AREF pin
    
    // Enable ADC and ADC complete interrupt, select ADC input clock
    ADCSRA |= _BV(ADEN) | _BV(ADIE) | 0b111 << ADPS0;
    return;
}

void adc_enable_pin(ADC_selection choice) {
    DIDR0 |= _BV(choice);
    return;
}

void adc_disable_pin(ADC_selection choice) {
    DIDR0 &= ~_BV(choice);
    return;
}

void adc_select_pin(ADC_selection choice) {
    ADMUX |= 0b1111;
    ADMUX &= 0xF0 | choice;
    return;
}

adc_result adc_read(void) {
    conversion_done = 0;
    ADCSRA |= _BV(ADSC);
    while (!conversion_done);
    return ADC;
}

ADC_selection adc_get_active_pin(void) {
    return (ADMUX & 0x0F);
}