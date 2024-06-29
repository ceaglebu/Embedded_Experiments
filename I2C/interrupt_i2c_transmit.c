// Interrupt-based I2C transmission.
// Can send multiple bytes. More structured than polling test.

#define __AVR_ATmega328P__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stddef.h>

#define STATUS_CODE (TWSR & 0xF8)

typedef struct {
    volatile uint8_t addr;
    volatile uint8_t* data;
    volatile size_t data_len;
    volatile size_t progress_idx;
} data_packet;

volatile enum {START, ADDR, DATA, STOP} state = STOP;
volatile data_packet current_data = (data_packet) {
    .addr = 0,
    .data = NULL,
    .data_len = 0,
    .progress_idx = 0
};

void i2c_transmit(data_packet* a_data) {
    while(state != STOP); // Wait for current transmission to finish. This probably should be a buffer instead to avoid hangs.
    current_data = *a_data; 

    state = START;
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE);
}

void i2c_failure() {
    // Very bad error handling for debugging. Obviously normally you don't want infinite hang in your ISR.
    PORTB |= _BV(PORTB5);
    while (1) {
        _delay_ms(3000);
    }
}

ISR(TWI_vect) {
    switch (state) {
        case START: 
            if (STATUS_CODE != 0x08) {
                i2c_failure();
                break;
            } 
            TWDR = current_data.addr << 1; // Bit 0 is R/W, 0 = Write
            state = ADDR;
            TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
            break;

        case ADDR:
            if (STATUS_CODE != 0x18 && STATUS_CODE != 0x20) {
                i2c_failure();
                break;
            }

            if (current_data.data_len == 0) {
                state = STOP;
                break;
            }

            TWDR = current_data.data[0];
            current_data.progress_idx += 1;
            state = DATA;
            TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
            break;

        case DATA:
            if (STATUS_CODE != 0x28 && STATUS_CODE != 0x30) {
                i2c_failure();
                break;
            }

            if (current_data.progress_idx == current_data.data_len) {
                state = STOP;
                break;
            }

            TWDR = current_data.data[current_data.progress_idx];
            current_data.progress_idx += 1;
            TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
            break;

        case STOP:
            if (STATUS_CODE != 0x28 && STATUS_CODE != 0x30 && STATUS_CODE != 0x18 && STATUS_CODE != 0x20) {
                i2c_failure();
            }
            TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN) | _BV(TWIE);
    }
}

int main (void) {
    // Initialize on-board light for debugging
    DDRB |= _BV(DDB5);
    PORTB &= ~_BV(PORTB5);

    // Initialize I2C
    // Set bit rate - 1 prescale, bit rate register = 144 
    // bit rate = 16mhz / (16 + 2(TWBR) * prescaler)
    //          = 16mhz / (16 + 144) = 100khz
    TWBR = 72;
    TWSR |= 0 << _BV(TWPS0);

    // Activate pull up resistors
    PORTC |= _BV(PORTC5) | _BV(PORTC4);

    // Enable interrupts
    sei();
    TWCR |= _BV(TWIE);

    // Start Transmission
    uint8_t data_array[1] = {0xAB};
    data_packet data = (data_packet) {.addr = 0x27, .data = data_array, .data_len = 1};
    i2c_transmit(&data);

    uint8_t data_array_2[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
    data_packet packet_2 = (data_packet) {.addr = 0x27, .data = data_array_2, .data_len = sizeof(data_array_2)};
    i2c_transmit(&packet_2);

    while (1) {

    }
}