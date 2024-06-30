#ifndef _I2C_HAL_H_

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

/************************************************ INSTRUCTIONS **************************************************/ 
/* 1. Global interrupts must be enabled                                                                         */
/* 2. i2c_init()                                                                                                */
/* 3. i2c_transmit_byte(uint8_t addr, uint8_t data)                      /* transmits byte to addressed device  */
/* 4. i2c_transmit_bytes(uint8_t addr, uint8_t * data, size_t len_data)  /* transmits bytes to addressed device */
/****************************************************************************************************************/ 

// Transmissions are BLOCKING

#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include <avr/interrupt.h>

void i2c_init();
bool i2c_transmit_byte(uint8_t addr, uint8_t data);
bool i2c_transmit_bytes(uint8_t addr, uint8_t * data, size_t len_data);

#endif