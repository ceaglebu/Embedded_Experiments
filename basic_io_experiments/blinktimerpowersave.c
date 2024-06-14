// --------------------------------- //
// POWER SAVE TIMER BLINK            //
// PUT PROCESSOR IN POWER SAVE MODE  //
// WHILE NOT BLINKING                //
// GREATLY REDUCE POWER USE          //
// --------------------------------- //

#define __AVR_ATmega328__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define CLOCKRATE 32768
#define PRESCALER 10
#define CLOCK_CYCLES (CLOCKRATE >> PRESCALER)

ISR(TIMER2_COMPA_vect) {
    PORTB ^= _BV(PORTB5);
    OCR2A = CLOCK_CYCLES;
    while (ASSR & (_BV(TCN2UB) | _BV(OCR2AUB) | _BV(TCR2AUB))) {
        // pause
    }
}
 
int main (void) {
    // Set light's pinMode to output
    DDRB |= _BV(PORTB5);
    
    // Select Async clock, select prescaled clock
    ASSR |= _BV(AS2);
    TCCR2B |= _BV(CS22) | _BV(CS21) | _BV(CS20);

    // Select CTC Mode
    TCCR2A |= _BV(WGM21);

    // Set Timer Value
    OCR2A = CLOCK_CYCLES;

    // Reset Timer
    TCNT2 = 0x0000;
    while (ASSR & (_BV(TCN2UB) | _BV(OCR2AUB) | _BV(TCR2AUB))) {
        // pause
    }
    // Reset potentially Corrupt interrupt bits and Request interrupt
    TIMSK2 &= ~(_BV(OCIE2B) | _BV(TOIE2));
    TIMSK2 |= _BV(OCIE2A);
    sei();

    while (true) {
        // Turn on power save mode
        // SMCR |= _BV(SM1) | _BV(SM0);
    }
}