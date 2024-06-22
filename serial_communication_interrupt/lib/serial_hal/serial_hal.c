#include "serial_hal.h"
#include "..\ring_buffer\ring_buffer.h"
#include <avr/interrupt.h>
#include <assert.h>

#define BAUDRATE_BYTES (103)

#define RING_BUFFER_SIZE (16)
uint8_t buffer[RING_BUFFER_SIZE];
ring_buffer transmit_buffer = (ring_buffer) {.buffer = buffer, .size = RING_BUFFER_SIZE};

static void uart_start_tx() {
    if (!ring_buffer_empty(&transmit_buffer)){
        UDR0 = ring_buffer_peek(&transmit_buffer); 
    } 
}

ISR(USART_UDRE_vect) {
    ring_buffer_pop(&transmit_buffer);
    
    if (!ring_buffer_empty(&transmit_buffer)) {
        uart_start_tx();
    } else {
        UCSR0B &= ~_BV(UDRIE0);
    }
}

void serial_begin() {
    UBRR0 = BAUDRATE_BYTES;
    // enable TX (and RX in future)
    UCSR0B |= _BV(TXEN0);
    // Set 8 bit length
    UCSR0B |= (3 << UCSZ00);
}

void serial_send_char(char character) {
    // Add Character to ring buffer
    bool new_tx = ring_buffer_empty(&transmit_buffer);
    ring_buffer_insert(&transmit_buffer, character);
    
    if (new_tx) {
        uart_start_tx();
        // Clear and enable serial transmission available interrupt.
        // Needs to happen after serial transmission initaited, or else ISR will always run
        UCSR0A &= ~_BV(UDRE0);
        UCSR0B |= _BV(UDRIE0);
    }
}

void serial_send_string(const char message[]) {
    for (size_t idx_char = 0; message[idx_char] != '\0'; idx_char += 1) {
        serial_send_char(message[idx_char]);
    }
}