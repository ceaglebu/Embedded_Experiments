#include "ring_buffer.h"

ring_buffer* ring_buffer_init(size_t size) {
    uint8_t * buffer = malloc(sizeof(*buffer) * size);
    ring_buffer* rb = malloc(sizeof(*rb));
    *rb = (ring_buffer) {.buffer = buffer, .head = 0, .size = size, .tail = 0};
}

void ring_buffer_free(ring_buffer** rb) {
    free((*rb)->buffer);
    (*rb)->buffer = NULL;
    free(*rb);
    *rb = NULL;
}

void ring_buffer_insert(ring_buffer* rb, uint8_t data) {
    rb->buffer[rb->head] = data;
    (rb->head) += 1;
    if (rb->head == rb->size) {
        rb->head = 0;
    }
}

uint8_t ring_buffer_peek(ring_buffer* rb) {
    return rb->buffer[rb->tail];
}

uint8_t ring_buffer_pop(ring_buffer* rb) {
    uint8_t data = rb->buffer[rb->tail];
    (rb->tail) += 1;
    if (rb->tail == rb->size) {
        rb->tail = 0;
    }
    return data;
}

bool ring_buffer_empty(ring_buffer* rb) {
    return rb->head == rb->tail;
}

bool ring_buffer_full(ring_buffer* rb) {
    size_t idx_after_head = rb->head + 1;
    if (idx_after_head == rb->size) {
        idx_after_head = 0;
    }
    return idx_after_head == rb->tail;
}