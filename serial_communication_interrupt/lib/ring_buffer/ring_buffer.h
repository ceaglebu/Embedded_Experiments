#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Simple 8-bit ring buffer.
// To use, define a uint8_t array, then create a ring_buffer struct with .buffer = *your array* and .size = *size of your array*

// ex:
// #define BUFFER_SIZE (10)
// uint8_t buffer_array[BUFFER_SIZE];
// ring_buffer rb = (ring_buffer) {.buffer = buffer_array, .size = BUFFER_SIZE};

typedef struct {
    uint8_t * buffer;
    size_t size;
    volatile size_t head;
    volatile size_t tail;
} ring_buffer;

void ring_buffer_insert(ring_buffer* rb, uint8_t data);
uint8_t ring_buffer_peek(ring_buffer* rb);
uint8_t ring_buffer_pop(ring_buffer* rb);

bool ring_buffer_empty(ring_buffer* rb);
bool ring_buffer_full(ring_buffer* rb);