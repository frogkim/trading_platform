#include "libnetwork_header.h"
// variables in server

#ifdef LINUX
int     socket;
#else
// --- Windows ---
SOCKET  g_listen;           
HANDLE  g_h_listen_thread    = NULL;
HANDLE	g_h_iocp             = NULL;
HANDLE  g_h_iocp_thread_pool = NULL;

CRITICAL_SECTION g_cs;
SYSTEM_INFO g_info;

#ifdef DEBUG
#include <stdio.h>
#endif
// --- Windows ---
#endif

//EnterCriticalSection(&g_cs);
//LeaveCriticalSection(&g_cs);
node_t* clients;
queue_t q_seats_int32;
queue_t q_recvs_buf_t;
queue_t q_sends_buf_t;

bool b_setup = false;               // limit to setup after initialization
bool b_is_stop = false;             
uint32_t size_client = 0;
uint32_t capacity_seats = 4000;
uint32_t capacity_recvs = 4000;
uint32_t capacity_sends = 4000;
uint32_t capacity_clients = 4000;
uint32_t n_iocp_threads = 0;
uint32_t n_iocp_port_threads = 0;   // automatic by os


// internal function declaration
bool mem_alloc(void);
void mem_release(void);
bool set_listen(void);
bool set_iocp(void);
void close_all_sockets();

uint32_t __stdcall func_thread_iocp(void* pParam);
uint32_t __stdcall func_thread_accept(void* pParam);

enum {
    close_all = 0,
    close_iocp_threadpool = 1,
    close_iocp_handle = 2,
    close_listen_thread = 3,
    close_listen_socket = 4,
};

// External functions
unsigned int __stdcall ServerInit(void)
{

    // get system data
    GetSystemInfo(&g_info);

    // initiate critical section
    InitializeCriticalSection(&g_cs);

    if (!mem_alloc()) {
        return false;
    }
    if (!set_listen()) {
        return false;
    }
    if (!set_iocp()) {
        return false;
    }

    b_setup = true;
    return true;
}

bool set_listen(void)
{
    // set listen socket
    g_listen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (g_listen == SOCKET_ERROR) {
        mem_release();
        return false;
    }

    // bind
    SOCKADDR_IN addrsvr;
    memset(&addrsvr, 0, sizeof(SOCKADDR_IN));
    addrsvr.sin_family = AF_INET;
    addrsvr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    addrsvr.sin_port = htons(25000);

    int result = 0;
    result = bind(g_listen, (SOCKADDR*)&addrsvr, sizeof(SOCKADDR_IN));
    if (result == SOCKET_ERROR) {
        ServerDestruct(close_listen_socket);
        return false;
    }
    result = listen(g_listen, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        ServerDestruct(close_listen_socket);
        return false;
    }

    // run threads
    g_h_listen_thread = CreateThread(NULL, 0, func_thread_accept, (LPVOID)NULL, 0, NULL);
    if (g_h_listen_thread == NULL) {
        ServerDestruct(close_listen_socket);
        return false;
    }

    return true;
}

bool set_iocp(void)
{
    // set iocp
    g_h_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, n_iocp_threads);
    if (g_h_iocp == NULL) {
        ServerDestruct(close_iocp_handle);
        return false;
    }

    g_h_iocp_thread_pool = CreateThreadpool(NULL);
    if (g_h_iocp_thread_pool == NULL) {
        ServerDestruct(close_iocp_threadpool);
        return false;
    }
    return true;
}



void __stdcall ServerDestruct(int step)
{
    switch (step){  // intended cascade
    case close_all:
    case close_iocp_threadpool:
        CloseThreadpool(g_h_iocp_thread_pool);
    case close_iocp_handle:
        CloseHandle(g_h_iocp);
    case close_listen_thread:
        shutdown(g_listen, SD_BOTH);
        closesocket(g_listen);
    case close_listen_socket:
        CloseHandle(g_h_listen_thread);
    default:
        break;
    }
}

void __stdcall ServerRun(void)
{
    b_is_stop = false;
}

void __stdcall ServerStop(void)
{
    b_is_stop = true;
}

unsigned int __stdcall ServerRunNode(unsigned int index)
{
    return 0;
}

unsigned int __stdcall ServerStopNode(unsigned int index)
{
    return 0;
}

unsigned int __stdcall ServerRemoveNode(unsigned int index)
{
    return 0;
}

unsigned int __stdcall ServerSend(unsigned int index, char* buf)
{
    return 0;
}

unsigned int __stdcall ServerRecv(unsigned int index, char* buf)
{
    return 0;
}

void __stdcall ServerSendAsync(unsigned int index, char* buf)
{
}

void __stdcall ServerRecvAsync(unsigned int index, char* buf)
{
}
//
//void __stdcall ServerSetBufferSize(unsigned int value)
//{
//    size_buffer = value;
//}
//
//void __stdcall ServerSetCapacity(unsigned int value)
//{
//    capacity = value;
//}
//
//void __stdcall ServerSetNumberIocpThreads(unsigned int value)
//{
//    n_iocp_threads = value;
//}
//
//void __stdcall ServerSetNumberIocpPortThreads(unsigned int value)
//{
//    n_iocp_port_threads = value;
//}
//

// Internal functions
bool mem_alloc(void)
{
    // create wait queue
    if (!init_queue(&q_seats_int32, capacity_seats, sizeof(int32_t))) {
        mem_release();
        return false;
    }
    if (!init_queue(&q_recvs_buf_t, capacity_recvs, sizeof(buf_t))) {
        mem_release();
        return false;
    }
    if (!init_queue(&q_sends_buf_t, capacity_sends, sizeof(buf_t))) {
        mem_release();
        return false;
    }

    clients = (node_t*)malloc(sizeof(node_t) * capacity_clients);
    if (clients == NULL) {
        mem_release();
        return false;
    }
    memset(clients, 0, sizeof(node_t) * capacity_clients);
    return true;
}

void mem_release()
{
    safe_release(clients);
    deinit_queue(&q_sends_buf_t);
    deinit_queue(&q_recvs_buf_t);
    deinit_queue(&q_seats_int32);
}

uint32_t __stdcall func_thread_iocp(void* pParam)
{
    DWORD			size_transfer = 0;
    node_t*         client = NULL;
    LPWSAOVERLAPPED	p_wol = NULL;
    BOOL			bResult;
#ifdef DEBUG
    printf("func_thread_iocp started.");
#endif
    while (true)
    {
        bResult = GetQueuedCompletionStatus( g_h_iocp, &size_transfer, (PULONG_PTR)&client, &p_wol, INFINITE);

        if (size_transfer > 0 && bResult == true) {
            // normal. create iocp again
            // do something when it received
            WSARecv(client->socket, &client->wsabuf, 1, &client->n_recv, &client->flag, &client->wol, NULL);


            // TODO: when WSARecv failed                    
            // WSAGetLastError() != WSA_IO_PENDING

        } else {
            // disconnected



            //if (bResult == true) {
            //    // disconnected correctly

            //} else {
            //    if (p_wol == NULL) {
            //        // failed to get packet
            //    } else {
            //        // disconnected by server or others
            //    }
            //}
        }

    }
    return 0;
}

uint32_t __stdcall func_thread_accept(void* pParam)
{
    SOCKET			h_client;
    SOCKADDR		ClientAddr;
    int				nAddrSize = sizeof(SOCKADDR);
#ifdef DEBUG
    puts("func_thread_accept started.");
#endif
    // TODO: WSAaccept function conditionally accepts. Find it.
    while ((h_client = accept(g_listen, &ClientAddr, &nAddrSize)) != INVALID_SOCKET)
    {
        uint32_t index;
        if (seats.size > 0) {
            EnterCriticalSection(&g_cs);
            index = seats.first;
            LeaveCriticalSection(&g_cs);
        } else {
            index = size_client;
            EnterCriticalSection(&g_cs);
            size_client++;
            LeaveCriticalSection(&g_cs);
        }

        clients[index].socket = h_client;
        memset(clients[index].wsabuf.buf, 0, size_buffer);
        memset(&clients[index].wol, 0, sizeof(WSAOVERLAPPED));
        clients[index].n_recv = 0;
        clients[index].flag = 0;
        CreateIoCompletionPort((HANDLE)h_client, g_h_iocp, (ULONG_PTR)&clients[index], 0);
        WSARecv(clients[index].socket, 
            &clients[index].wsabuf,
            1,                          // number of wsabuf
            &clients[index].n_recv,
            &clients[index].flag,       // flag for modifying the behavior of the WSARecv
            &clients[index].wol,
            NULL                        // callback
        );
        // TODO: Error check
    }

    // thread end when all clients are disconnected.
    return 0;
}
