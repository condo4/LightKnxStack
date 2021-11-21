#pragma once
#include <stdint.h>

typedef struct
{
    volatile uint8_t *buffer;
    int head;
    int tail;
    const int maxlen;
    int overflow;
} ring_buffer;

int ring_buffer_push(ring_buffer *c, uint8_t data)
{
    int next;

    next = c->head + 1; // next is where head will point to after this write.
    if (next >= c->maxlen)
        next = 0;

    if (next == c->tail) // if the head + 1 == tail, circular buffer is full
    {
        ++(c->overflow);
        return -1;
    }

    c->buffer[c->head] = data; // Load data and then move
    c->head = next; // head to next data offset.
    return 0; // return success to indicate successful push.
}

int ring_buffer_pop(ring_buffer *c, uint8_t *data)
{
    int next;

    if (c->head == c->tail) // if the head == tail, we don't have any data
        return -1;

    next = c->tail + 1; // next is where tail will point to after this read.
    if (next >= c->maxlen)
        next = 0;

    *data = c->buffer[c->tail]; // Read data and then move
    c->tail = next; // tail to next offset.
    return 0; // return success to indicate successful push.
}

int ring_buffer_empty(ring_buffer *c)
{
    return c->head == c->tail;
}

#define RING_BUFFER_DEF(x, y)                                                                      \
    volatile uint8_t x##_data_space[y];                                                            \
    ring_buffer x = { .buffer = x##_data_space, .head = 0, .tail = 0, .maxlen = y, .overflow = 0 }
