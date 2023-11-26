#include "libnetwork.h"

#ifndef LIBNETWORK_HEADER_H_
#define LIBNETWORK_HEADER_H_

// common
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <queue>
#define safe_release(p) if(p) free(p)

#ifdef LINUX
// Linux
#include <sys/socket.h>
// --- end --- Linux
#else
// Windows
#include <tchar.h>
#include <winSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <ws2tcpip.h>
#include <Windows.h>

//Thread functions
DWORD WINAPI ServerIocpThreadLoop(LPVOID pParam);
DWORD WINAPI ServerListenThreadLoop(LPVOID pParam);
DWORD WINAPI ClientIocpThreadLoop(LPVOID pParam);
// --- end --- Windows
#endif

const uint32_t BASE_CAPACITY = 100;

struct node_st {
    bool    occupied;
    bool    stop;
    uint32_t    index;
#ifdef LINUX
    int     socket;
#else   // Windows
    SOCKET  socket;
#endif
    char* buffer;
};

struct client_st {
    bool isRun;
#ifdef LINUX
    int     socket;
#else   // Windows
    SOCKET  socket;
#endif
    char* buffer;
};

struct client_info_st {
    char ip;
    uint32_t buffer_size;
    uint32_t MAX_IOCP_THREADS;
};

struct server_st {
    bool isRun;
#ifdef LINUX
    int     socket;
#else   // Windows
    SOCKET listen_fd;
#endif    
    void* mem;
    std::queue<uint32_t> prior_seats;
    std::vector<struct client_st*> client_list;
};

struct server_info_st {
    uint32_t buffer_size;
    uint32_t max_iocp_threads;
    uint32_t max_capacity;
    uint32_t default_client_list_size;
};

// global variable
// struct client_info_st client_info;
// struct server_info_st server_info;
// struct client_st* pClient;

#endif