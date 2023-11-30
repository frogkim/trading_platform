#include "libnetwork_header.h"
// variables in server

#ifdef LINUX
int     socket;
#else
#include <wdm.h>

// --- Windows ---
SOCKET  g_listen;           
HANDLE  g_h_listen_thread    = NULL;
HANDLE	g_h_iocp             = NULL;
PTP_POOL g_h_iocp_thread_pool = NULL;
PTP_CLEANUP_GROUP g_h_iocp_thread_pool_cleanupgroup = NULL;
PTP_WORK g_h_iocp_thread_pool_work = NULL;

SYSTEM_INFO g_info;

#ifdef DEBUG
#include <stdio.h>
#endif
// --- Windows ---
#endif
uint32_t number_iocp_threads = 0;

bool b_setup = false;               // limit to setup after initialization
bool b_is_stop = false;
bool b_is_terminate = false;

// clients
node_t* clients;
uint32_t size_client = 0;
uint32_t capacity_clients = 4000;

// queues
queue_t q_seats_int32;
queue_t q_recvs_node;
queue_t q_sends_node;

CRITICAL_SECTION g_cs_seats;
CRITICAL_SECTION g_cs_recvs;
CRITICAL_SECTION g_cs_sends;

HANDLE g_evt_recvs;
HANDLE g_evt_sends;
KSPIN_LOCK g_spinlock_recvs;
KSPIN_LOCK g_spinlock_sends;

uint32_t capacity_seats = 4000;
uint32_t capacity_recvs = 10000;
uint32_t capacity_sends = 10000;

// queue for seats has only uint32s.
uint32_t size_buffer_recvs_max = 8192;
uint32_t size_buffer_sends_max = 8192;


// internal function declaration
bool set_global(void);
bool set_listen(void);
bool set_iocp(void);
void close_all_sockets();
void release_all(void);

uint32_t __stdcall func_thread_accept(void* pParam);
uint32_t __stdcall CALLBACK func_thread_iocp(void* pParam);
uint32_t __stdcall func_recvs_queue(void* pParam);
uint32_t __stdcall func_sends_queue(void* pParam);

enum {
    close_all,
    close_iocp_threadpool_cleanupgroup_members,
    close_iocp_threadpool_work,
    close_iocp_threadpool_cleanupgroup,
    close_iocp_threadpool,
    close_iocp_handle,
    close_listen_thread,
    close_listen_socket,
};

// External functions
unsigned int __stdcall ServerInit(void)
{
    if (!set_global()) {
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

void __stdcall ServerDestruct(int step)
{
    switch (step) {  // intended cascade
    case close_all:
        CloseThreadpoolCleanupGroupMembers(g_h_iocp_thread_pool_cleanupgroup, FALSE, NULL);
    case close_iocp_threadpool_cleanupgroup_members:
        CloseThreadpoolCleanupGroup(g_h_iocp_thread_pool_cleanupgroup);
    case close_iocp_threadpool_work:
    case close_iocp_threadpool_cleanupgroup:
        CloseThreadpool(g_h_iocp_thread_pool);
    case close_iocp_threadpool:
        CloseHandle(g_h_iocp);
    case close_iocp_handle:
        CloseHandle(g_h_listen_thread);
    case close_listen_thread:
        shutdown(g_listen, SD_BOTH);
        closesocket(g_listen);    case close_listen_socket:
    default:
        break;
    }
    release_all();
}

void __stdcall ServerRun(void)
{
    b_is_stop = false;
}

void __stdcall ServerStop(void)
{
    b_is_stop = true;
}

void __stdcall ServerTerminate(void)
{
    b_is_terminate = true;
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
bool set_global(void)
{
    enum {
        release_clients_list,
        release_q_sends_node,
        release_q_recvs_node,
        release_q_queue
    };
    // get system data
    GetSystemInfo(&g_info);

    // create wait queue
    if (!init_queue(&q_seats_int32, capacity_seats, sizeof(int32_t))) {
        release_all();
        return false;
    }

    if (!init_queue(&q_recvs_node, capacity_recvs, sizeof(node_t))) {
        release_all();
        return false;
    }

    if (!init_queue(&q_sends_node, capacity_sends, sizeof(node_t))) {
        release_all();
        return false;
    }

    clients = (node_t*)malloc(sizeof(node_t) * capacity_clients);
    if (clients == NULL) {
        release_all();
        return false;
    }
    memset(clients, 0, sizeof(node_t) * capacity_clients);
    for (uint32_t i = 0; i < capacity_clients; i++) {
        clients[i].index = i;
    }
    
    InitializeCriticalSection(&g_cs_seats);
    InitializeCriticalSection(&g_cs_recvs);
    InitializeCriticalSection(&g_cs_sends);

    KeInitializeSpinLock();
    KSPIN_LOCK g_spinlock_recvs;
    KSPIN_LOCK g_spinlock_sends;
    g_evt_recvs = CreateEvent(NULL, TRUE, FALSE, TEXT("recvs_event"));
    g_evt_sends = CreateEvent(NULL, TRUE, FALSE, TEXT("sends_event"));
    
    return true;
}

bool set_listen(void)
{
    // set listen socket
    g_listen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (g_listen == SOCKET_ERROR) {
        release_all();
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
    DWORD n_iocp_threads = 0;   // set automatic
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
    if (number_iocp_threads == 0) {
        number_iocp_threads = g_info.dwNumberOfProcessors;
    }
    SetThreadpoolThreadMaximum(g_h_iocp_thread_pool, number_iocp_threads);
    SetThreadpoolThreadMinimum(g_h_iocp_thread_pool, 1);

    g_h_iocp_thread_pool_cleanupgroup = CreateThreadpoolCleanupGroup();

    TP_CALLBACK_ENVIRON CallBackEnviron;
    InitializeThreadpoolEnvironment(&CallBackEnviron);

    SetThreadpoolCallbackPool(&CallBackEnviron, g_h_iocp_thread_pool);
    SetThreadpoolCallbackCleanupGroup(&CallBackEnviron, g_h_iocp_thread_pool_cleanupgroup, NULL);

    if (g_h_iocp_thread_pool_cleanupgroup == NULL) {
        ServerDestruct(close_iocp_threadpool_cleanupgroup);
        return false;
    }
    g_h_iocp_thread_pool_work = CreateThreadpool(func_thread_iocp, NULL, &CallBackEnviron);
    if (g_h_iocp_thread_pool_work == NULL) {
        ServerDestruct(close_iocp_threadpool_work);
        return false;
    }
    // start thread pool
    SubmitThreadpoolWork(g_h_iocp_thread_pool_work);

    //--- Create Timer
    //PTP_TIMER_CALLBACK timercallback = func_callback_timer;
    //PTP_TIMER timer = CreateThreadpoolTimer(timercallback, NULL, &CallBackEnviron);
    //FILETIME FileDueTime;
    //ULARGE_INTEGER ulDueTime;
    //ulDueTime.QuadPart = (ULONGLONG)-(1 * 10 * 1000 * 1000);
    //FileDueTime.dwHighDateTime = ulDueTime.HighPart;
    //FileDueTime.dwLowDateTime = ulDueTime.LowPart;
    //SetThreadpoolTimer(timer, &FileDueTime, 0, 0);

    return true;
}

void release_all()
{
    DeleteCriticalSection(&g_cs_seats);
    DeleteCriticalSection(&g_cs_recvs);
    DeleteCriticalSection(&g_cs_sends);
    DeleteCriticalSection(&g_cs_evt_recvs);
    DeleteCriticalSection(&g_cs_evt_sends);

    safe_release(clients);
    deinit_queue(&q_sends_node);
    deinit_queue(&q_recvs_node);
    deinit_queue(&q_seats_int32);
}

uint32_t __stdcall func_thread_accept(void* pParam)
{
    SOCKET			h_client;
    SOCKADDR		ClientAddr;
    int				nAddrSize = sizeof(SOCKADDR);
    uint32_t        index = 0;
#ifdef DEBUG
    puts("func_thread_accept started.");
#endif
    // TODO: WSAaccept function conditionally accepts. Find it.
    while ((h_client = accept(g_listen, &ClientAddr, &nAddrSize)) != INVALID_SOCKET)
    {

        if (q_seats_int32.size > 0) {
            EnterCriticalSection(&g_cs_seats);
            q_seats_int32.get_front_or_null(&q_seats_int32, (int*)&index);
            LeaveCriticalSection(&g_cs_seats);
            if (&index == NULL) {
                // TODO: terminate the program
            }
        } else {
            EnterCriticalSection(&g_cs_seats);
            index = q_seats_int32.size;
            q_seats_int32.size++;
            LeaveCriticalSection(&g_cs_seats);
        }

        clients[index].socket = h_client;
        memset(clients[index].wsabuf.buf, 0, size_buffer_recvs_max);
        memset(&clients[index].wol, 0, sizeof(WSAOVERLAPPED));
        clients[index].n_recv = 0;
        clients[index].flag = 0;

        CreateIoCompletionPort((HANDLE)clients[index].socket, g_h_iocp, (ULONG_PTR)&clients[index], 0);
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

uint32_t __stdcall CALLBACK func_thread_iocp(void* pParam)
{
    DWORD			size_transfer = 0;
    node_t*         client = NULL;
    LPWSAOVERLAPPED	p_wol = NULL;
    BOOL			result;
#ifdef DEBUG
    printf("func_thread_iocp started.");
#endif
    while (true)
    {
        result = GetQueuedCompletionStatus( g_h_iocp, &size_transfer, (PULONG_PTR)&client, &p_wol, INFINITE);

        // TODO: investigate about result
        if (result == true) {
            if (size_transfer > 0) {
                // normal. create iocp again
                EnterCriticalSection(&g_cs_recvs);
                q_recvs_node.set_tail(&q_sends_node, &client);
                LeaveCriticalSection(&g_cs_recvs);
                SetEvent(g_evt_recvs);
                WSARecv(client->socket, &client->wsabuf, 1, &client->n_recv, &client->flag, &client->wol, NULL);
                // TODO: when WSARecv failed                    
                // WSAGetLastError() != WSA_IO_PENDING

            } else {
                //        // failed to get packet
            }
        } else {
            if (p_wol == NULL) {
                // disconnected correctly
                shutdown(client->socket, SD_BOTH);
                closesocket(client->socket);
                EnterCriticalSection(&g_cs_seats);
                q_seats_int32.set_tail(&client, q_seats_int32.size);
                LeaveCriticalSection(&g_cs_seats);
            } else {
                // disconnected by server or others
            }
        }

    }
    return 0;
}

uint32_t __stdcall func_recvs_queue(void* pParam)
{
    DWORD dw_event;
    node_t client;

    while (true) {
        dw_event = WaitForSingleObject(g_evt_recvs, INFINITE);
        if (b_is_terminate) {
            break;
        }
        while (true) {
            EnterCriticalSection(&g_cs_seats);
            if (q_recvs_node.size > 0) {
                q_recvs_node.get_front_or_null(&q_recvs_node, &client);
            } else {
                break;
            }
            LeaveCriticalSection(&g_cs_seats);

            SetEvent(g_evt_sends);
        }
        ResetEvent(g_evt_recvs);
    }
    return 0;
}
uint32_t __stdcall func_sends_queue(void* pParam)
{
    DWORD dw_event;
    while (true) {
        dw_event =WaitForSingleObject(g_evt_sends, INFINITE);
        if (b_is_terminate) {
            break;
        }


    }
    
    return 0;
}
