#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif
#include <avr/io.h>
#include <stddef.h>
#include <avr/sleep.h>

// Serial communication driver. Uses normal async mode (no 2X). Uses 8 bit, 1 stop bit, no parity bit data format. 9600 baud.
// Buffers transmission up to 16 bytes / characters. Overloading buffer results in 16-chacter long nonsense, but does not break anything.
// Printing is non blocking. Does not wait for transmission to complete before returning control to calling function.

void serial_begin();
void serial_send_char(char character);
void serial_send_string(const char message[]);
volatile uint8_t serial_read_char();
// Go into power save mode while waiting for next transmission
volatile uint8_t serial_read_char_sleep(uint8_t mode_vector);
void serial_attach(volatile uint8_t*);