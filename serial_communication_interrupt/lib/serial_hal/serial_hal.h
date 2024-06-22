#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif
#include <avr/io.h>
#include <stddef.h>

void serial_begin();
void serial_send_char(char character);
void serial_send_string(const char message[]);