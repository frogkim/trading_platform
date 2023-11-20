#ifndef TRADING_PLATFORM_NETWORK_HEADER_H_
#define TRADING_PLATFORM_NETWORK_HEADER_H_

#include "network.h"
#include <stdlib.h>
#include <vector>
#include <queue>

#define safe_release(p) if(p) free(p)

const UINT BASE_CAPACITY = 100;

struct node_st {
    bool    occupied;
    bool    stop;
    UINT    index;
    SOCKET  socket;
    char* buffer;
};

struct client_st {
    bool isRun;
    SOCKET socket;
    char* buffer;
};

struct client_info_st {
    char ip;
    UINT buffer_size;
    UINT MAX_IOCP_THREADS;
};

struct server_st {
    bool isRun;
    SOCKET listen_fd;
    void* mem;
    std::queue<UINT> prior_seats;
    std::vector<struct client_st*> client_list;
};

struct server_info_st {
    UINT buffer_size;
    UINT max_iocp_threads;
    UINT max_capacity;
    UINT default_client_list_size;
};

//Thread functions
DWORD WINAPI ServerIocpThreadLoop(LPVOID pParam);
DWORD WINAPI ServerListenThreadLoop(LPVOID pParam);
DWORD WINAPI ClientIocpThreadLoop(LPVOID pParam);
#endif