#include <avr/io.h>
#include <stddef.h>

#define BAUDRATE_BYTES 103

void serial_begin();
void serial_send_char(char character);
void serial_send_string(const char message[]);