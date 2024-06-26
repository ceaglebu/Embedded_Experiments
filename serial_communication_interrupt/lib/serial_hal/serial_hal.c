#include "serial_hal.h"
#include "..\ring_buffer\ring_buffer.h"
#include <avr/interrupt.h>

#define BAUDRATE_BYTES (103)

#define RING_BUFFER_SIZE (16)
uint8_t buffer[RING_BUFFER_SIZE];
ring_buffer transmit_buffer = (ring_buffer) {.buffer = buffer, .size = RING_BUFFER_SIZE};

volatile bool request_read = false;
volatile uint8_t read_data = 'a';
volatile volatile uint8_t * attached_data = NULL;

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

ISR(USART_RX_vect) {
    // Only care about data sent when listening. Other data is tossed out to clear interrupt.
    // request_read means serial_read_char was called.
    // attached_data != NULL means a variable was attached to represent serial receipts.
    read_data = UDR0;
    if (request_read) {
        request_read = false;
    }
    if (attached_data != NULL) {
        *attached_data = read_data;
    }
    PORTB |= _BV(PORTB5);
}

ISR(PCINT2_vect){
    // PORTB ^= _BV(PORTB5);
}

void serial_begin() {
    UBRR0 = BAUDRATE_BYTES;
    // enable TX (and RX in future)
    UCSR0B |= _BV(TXEN0) | _BV(RXEN0);
    // Set 8 bit length
    UCSR0C |= (3 << UCSZ00);
    // Enable receiving interrupt
    UCSR0B |= _BV(RXCIE0);
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

volatile uint8_t serial_read_char() {
    request_read = true;
    while (request_read);
    return read_data;
}

volatile uint8_t serial_read_char_sleep(uint8_t mode_vector) {
    // Flag to tell if message received since requesting a read
    request_read = true;

    // Enable pin 16 (D0 / RX pin) interrupt so it wakes when receiving message
    bool PCINT16_before = PCMSK2 & _BV(PCINT16);
    bool PCIE2_before = PCICR & _BV(PCIE2);
    PCMSK2 |= _BV(PCINT16);
    PCICR |= _BV(PCIE2);

    //Put mc to sleep
    set_sleep_mode(mode_vector);
    sleep_enable();
    while (request_read) {
        sleep_cpu(); // Go back to sleep as long as message hasn't been received
    }
    sleep_disable();

    // Revert pin 16 interrupt settings
    PCMSK2 |= PCINT16_before << PCINT16;
    PCICR |= PCIE2_before << PCIE2;
    return read_data;
}

void serial_attach(volatile uint8_t * data) {
    attached_data = data;
}