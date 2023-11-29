#ifndef LIBNETWORK_QUEUE
#define LIBNETWORK_QUEUE
typedef struct _queue_st {
    unsigned int front;
    unsigned int tail;
    unsigned int capacity;
    unsigned int size;
    unsigned int data_size;
    void (*set_tail)(struct _queue_st*, void*);
    void (*get_front)(struct _queue_st*, void*);
#ifdef LINUX
#else
    __declspec(align(8)) int* buffer;
#endif
} queue_t, * p_queue_t;

unsigned int init_queue(p_queue_t this, unsigned int capacity, unsigned int data_size);
void deinit_queue(p_queue_t this);

#endif // --- LIBNETWORK_QUEUE ---