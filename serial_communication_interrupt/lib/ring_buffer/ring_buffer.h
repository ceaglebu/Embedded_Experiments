#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

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