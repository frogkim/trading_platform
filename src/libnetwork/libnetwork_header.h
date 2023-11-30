#include "libnetwork.h"
#include "libnetwork_queue.h"

#ifndef LIBNETWORK_HEADER_H_
#define LIBNETWORK_HEADER_H_

// common
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#define     bool    uint32_t
#define     true    1
#define     false   0

#ifdef LINUX
// Linux
#include <sys/socket.h>
// --- end --- Linux
#else
// Windows
#include <tchar.h>
#define ALIGN8 __declspec(align(8))
#define safe_release(p) if(p) free(p)

ALIGN8 typedef struct _node_st {
    uint32_t is_stop;
    uint32_t index;
    SOCKET  socket;
    WSABUF  wsabuf;
    uint32_t n_recv;
    uint32_t flag;
    WSAOVERLAPPED wol;
    struct timespec ts;                     // timespec_get(&ts, TIME_UTC);
    void*   (*callback)(void*);
} node_t, * p_node_t;
// --- end --- Windows
#endif




#endif // LIBNETWORK_HEADER_H_