// ADC Hardware abstraction layer - reusable mini library to get ADCs working

/*********************************** INSTRUCTIONS *****************************************/ 
/* 1. Global interrupts must be enabled                                                   */
/* 2. adc_init()                                                                          */
/* 3. adc_enable(ADC_X)     /* for all ADCs that will be used                             */
/* 4. adc_select_pin(ADC_X) /* select what ADC adc_read() will read                       */
/* 5. adc_read(ADC_X)       /* returns value 0-1023 from selected adc                     */
/* 6. adc_get_active_pin()  /* if needed - returns ADC_selection of selected / active ADC */
/******************************************************************************************/ 

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