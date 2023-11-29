#include "libnetwork_queue.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define bool uint32_t
#define true 1
#define false 0

static void  _set_tail(p_queue_t, void*);
static void  _get_front(p_queue_t, void*);

bool init_queue(p_queue_t this, uint32_t capacity, uint32_t data_size)
{
    if (capacity == 0 || data_size == 0) {
        return false;
    }

    this->front = 0;
    this->tail = 0;
    this->capacity = capacity;
    this->size = 0;
    this->data_size = data_size;
    this->set_tail = _set_tail;
    this->get_front = _get_front;
    uint64_t size_assign = (uint64_t)data_size * (uint64_t)capacity;
    this->buffer = malloc(size_assign);
    return (this->buffer != NULL) ? true : false;
}

void deinit_queue(p_queue_t this)
{
    if (this != NULL && this->buffer != NULL) {
        free(this->buffer);
    }
}

void _set_tail(p_queue_t this, void* data)
{
    memcpy(data, &this->buffer[this->tail], this->data_size);
    this->tail++;
    this->size++;

    if (this->tail == this->capacity) {
        this->tail = 0;
    }
}

void _get_front(p_queue_t this, void* data)
{
    if (this->size == 0) {
        data = NULL;
        return;
    }
    memcpy(data, &this->buffer[this->front], this->data_size);
    this->front++;
    this->size--;
    if (this->front == this->capacity) {
        this->front = 0;
    }
}
