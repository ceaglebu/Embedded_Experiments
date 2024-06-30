#include "i2c_hal.h"

// Optimizations to make
// - try inlines

static inline void send_start_signal();
static inline void send_address(uint8_t addr);
static inline void send_byte(uint8_t byte); 
static inline void send_stop_signal();
static inline uint8_t get_status_code();

typedef struct _state {
    enum {START, ADDR, DATA, STOP} transmit_state;
    uint8_t address;
    uint8_t * a_data; 
    size_t len_data;
    size_t idx_cur_transmit;
} state; 

volatile state current_state = (volatile state) {
    .transmit_state = STOP,
    .address = 0,
    .a_data = NULL,
    .len_data = 0,
    .idx_cur_transmit = 0
};

uint8_t single_byte_register; // Single byte transmission buffer so after transmit_byte's stack is cleared, single byte can still exist

ISR(TWI_vect) {
    PORTB |= _BV(PORTB5);
    switch (current_state.transmit_state) {
        case START:
            // Start bit sent, send address
            send_address(current_state.address);
            current_state.transmit_state = ADDR;
            break;
        
        case ADDR:
            // Address sent, change to data mode 
            current_state.transmit_state = DATA;
            break;
        
        case DATA:
            // Byte transmission complete or starting, send next byte if available
            if (current_state.idx_cur_transmit == current_state.len_data) {
                current_state.transmit_state = STOP;
                break;
            }

            send_byte(current_state.a_data[current_state.idx_cur_transmit]);
            current_state.idx_cur_transmit += 1;
            break;
        
        case STOP:
            send_stop_signal();
            break;
    }
}

// Initialize I2C
void i2c_init() {
    // Set bit rate - No prescale, bit rate register = 144 
    // bit rate = 16mhz / (16 + 2(TWBR) * prescaler)
    //          = 16mhz / (16 + 144) = 100khz
    TWBR = 72;
    // TWSR |= 0 << _BV(TWPS0); // No prescaler. Including for clarity, commenting out for optimization.

    // Activate pull up resistors
    PORTC |= _BV(PORTC5) | _BV(PORTC4);

    // debugging
    DDRB |= _BV(DDB5);
    PORTB &= ~_BV(PORTB5);
}

bool i2c_transmit_byte(uint8_t addr, uint8_t data) {
    single_byte_register = data;
    i2c_transmit_bytes(addr, &single_byte_register, 1);
}

bool i2c_transmit_bytes(uint8_t addr, uint8_t * a_data, size_t len_data) {
    // Make information available to ISR
    current_state = (volatile state) {
        .transmit_state = START,
        .address = addr,
        .a_data = a_data,
        .len_data = len_data,
        .idx_cur_transmit = 0
    };

    // Start transmission
    send_start_signal();
    
    // Wait for transmission to end
    while (current_state.transmit_state != STOP);
}

static void send_start_signal() {
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN) | _BV(TWIE);
}

static void send_address(uint8_t addr) {
    send_byte(addr << 1);
}

static void send_byte(uint8_t data) {
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWIE);
}

static void send_stop_signal() {
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN) | _BV(TWIE);
}

static uint8_t get_status_code() {
    return TWSR & 0xF8;
}