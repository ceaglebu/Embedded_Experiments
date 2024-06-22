#include "ring_buffer.h"
#include <assert.h>
#include <stdio.h>

bool test_ring_buffer_insert_one() {
    uint8_t buffer[5];
    ring_buffer rb = (ring_buffer) {.buffer = buffer, .size = 5};
    ring_buffer_insert(&rb, 'a');
    assert(rb.buffer[0] == 'a');
    assert(rb.head == 1);
    assert(rb.tail == 0);
    return true;
}

bool test_ring_buffer_insert_3() {
    uint8_t buffer[5];
    ring_buffer rb = (ring_buffer) {.buffer = buffer, .size = 5};
    ring_buffer_insert(&rb, 'a');
    ring_buffer_insert(&rb, 'b');
    ring_buffer_insert(&rb, 'c');
    assert(rb.buffer[0] == 'a');
    assert(rb.buffer[1] == 'b');
    assert(rb.buffer[2] == 'c');
    assert(rb.head == 3);
    assert(rb.tail == 0);
    return true;
}

bool test_ring_buffer_insert_5() {
    uint8_t buffer[5];
    ring_buffer rb = (ring_buffer) {.buffer = buffer, .size = 5};
    ring_buffer_insert(&rb, 'a');
    ring_buffer_insert(&rb, 'b');
    ring_buffer_insert(&rb, 'c');
    ring_buffer_insert(&rb, 'd');
    ring_buffer_insert(&rb, 'e');
    assert(rb.buffer[0] == 'a');
    assert(rb.buffer[1] == 'b');
    assert(rb.buffer[2] == 'c');
    assert(rb.buffer[3] == 'd');
    assert(rb.buffer[4] == 'e');
    assert(rb.head == 0);
    assert(rb.tail == 0);
    return true;
}

bool test_ring_buffer_pop_one() {
    uint8_t buffer[5];
    ring_buffer rb = (ring_buffer) {.buffer = buffer, .size = 5};
    ring_buffer_insert(&rb, 'a');
    ring_buffer_pop(&rb);
    assert(rb.head == 1);
    assert(rb.tail == 1);
    assert(ring_buffer_empty(&rb));
    return true;
}

bool test_ring_buffer_pop_three() {
    uint8_t buffer[5];
    ring_buffer rb = (ring_buffer) {.buffer = buffer, .size = 5};
    assert(ring_buffer_empty(&rb));
    ring_buffer_insert(&rb, 'a');
    ring_buffer_insert(&rb, 'b');
    ring_buffer_insert(&rb, 'c');
    ring_buffer_insert(&rb, 'd');
    assert(ring_buffer_pop(&rb) == 'a');
    assert(ring_buffer_pop(&rb) == 'b');
    assert(ring_buffer_pop(&rb) == 'c');
    assert(rb.head == 4);
    assert(rb.tail == 3);
    return true;
}

bool test_ring_buffer_pop_five() {
    uint8_t buffer[5];
    ring_buffer rb = (ring_buffer) {.buffer = buffer, .size = 5};
    ring_buffer_insert(&rb, 'a');
    assert(!ring_buffer_empty(&rb));
    ring_buffer_insert(&rb, 'b');
    ring_buffer_insert(&rb, 'c');
    ring_buffer_insert(&rb, 'd');
    assert(ring_buffer_full(&rb));
    ring_buffer_insert(&rb, 'e');
    assert(ring_buffer_pop(&rb) == 'a');
    assert(ring_buffer_pop(&rb) == 'b');
    assert(ring_buffer_pop(&rb) == 'c');
    assert(ring_buffer_pop(&rb) == 'd');
    assert(ring_buffer_pop(&rb) == 'e');
    assert(rb.head == 0);
    assert(rb.tail == 0);
    assert(ring_buffer_empty(&rb));
    return true;
}

int main(void) {
    test_ring_buffer_insert_one();
    test_ring_buffer_insert_3();
    test_ring_buffer_insert_5();
    printf("Passed test_ring_buffer_insert()\n");
    test_ring_buffer_pop_one();
    test_ring_buffer_pop_three();
    test_ring_buffer_pop_five();
    printf("Passed test_ring_buffer_pop()\n");
}
