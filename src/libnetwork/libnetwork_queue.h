#ifndef LIBNETWORK_QUEUE
#define LIBNETWORK_QUEUE
/*
        Safe thread queue

        what kind of mothod will be used?
        When a user store data in queue, parameters copied in queue's buffer.
        When a user retrieve data from queue, paramters are copied from queue's buffer.
        Data parameter sometimes can be too large to use spin_lock. The worst scenario is like this. 
        When a user tried to retrieve a data, but the data is under copying, then it will make a crush.

        In this queue, CRITICAL SECTION is used for Windows OS and Mutex for Linux system.
*/

typedef struct _queue_st {
    unsigned int front;
    unsigned int tail;
    unsigned int capacity;
    unsigned int size;
    unsigned int data_size;
    void (*set_tail)(struct _queue_st*, void*);
    void (*get_front_or_null)(struct _queue_st*, void*);
    // buffer
#ifdef LINUX
#else
    __declspec(align(8)) void* buffer;
#endif
} queue_t, * p_queue_t;

unsigned int init_queue(p_queue_t this, unsigned int capacity, unsigned int data_size);
void deinit_queue(p_queue_t this);

#endif // --- LIBNETWORK_QUEUE ---