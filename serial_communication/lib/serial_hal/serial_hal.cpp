#include "serial_hal.h"

void serial_begin() {
    UBRR0 = BAUDRATE_BYTES;
    // enable TX (and RX in future)
    UCSR0B |= _BV(TXEN0);
    // Set 8 bit length
    UCSR0B |= (3 << UCSZ00);
}

void serial_send_char(char character) {
    // Wait for UDRE0 bit to clear
    // This means data register is OK to write to
    while (!(UCSR0A & _BV(UDRE0)));
    UDR0 = character;
}

void serial_send_string(const char message[]) {
    for (size_t idx_char = 0; message[idx_char] != '\0'; idx_char += 1) {
        serial_send_char(message[idx_char]);
    }
}