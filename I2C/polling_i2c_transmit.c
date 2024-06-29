// BASIC I2C Test
// Uses polling to send a single byte, basic message
// No error handling
// Master Transmitter Mode

#define __AVR_ATmega328P__
#include <avr/io.h>
#include <util/delay.h>

void i2c_error() {
    PORTB |= _BV(PORTB5);
    while (1) {
        _delay_ms(3000);
    }
}

int main (void) {

    // Initialize light for debugging
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

    // Send message
    // Send START signal
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN);

    // Wait for START to be sent
    while (!(TWCR & _BV(TWINT)));

    // Check that start was sent successfully
    if ((TWSR & 0xF8) != 0x08) {
        i2c_error();
    }

    // Load address + write bit into data register
    // Address = 0x27, write = 0
    TWDR = 0x27 << 1 | 0x00;

    // Load appropriate control signnals into control register to send address
    // TWSTA (start control) must also be turned off. Easier to just assign whole
    //      register value
    TWCR = _BV(TWINT) | _BV(TWEN);

    // Wait for address to be sent and ACK received. ACK will not be received during testing
    while (!(TWCR & _BV(TWINT)));

    if ((TWSR & 0xF8) != 0x18 && (TWSR & 0xF8) != 0x20) {
        i2c_error();
    }

    // Load data into data register
    TWDR = 0xAB;

    // Send data
    TWCR = _BV(TWINT) | _BV(TWEN);

    // Wait for data to be sent and check that status indicates data acknowledged
    while (!(TWCR & _BV(TWINT)));
    if ((TWSR & 0xF8) != 0x28 && (TWSR & 0xF8) != 0x30) {
        i2c_error();
    }

    // Send STOP signal
    TWCR |= _BV(TWINT) | _BV(TWSTO) | _BV(TWEN);

    // End transmission once STOP is done

    if (!(TWCR & _BV(TWINT))) {
        PORTB |= _BV(PORTB5);
    }

    while (1) {
        
    }
}