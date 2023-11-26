#include "libnetwork_header.h"

// global variable
bool bSetup = false;

// External functions
void ServerSetup(unsigned int buf_size, unsigned int max_iocp_threads, unsigned int max_server_capacity, unsigned int default_client_list_size)
{
    // server_info.buffer_size = buf_size;
    // server_info.max_iocp_threads = max_iocp_threads;
    // server_info.max_capacity = max_server_capacity;
    // server_info.default_client_list_size;
    bSetup = true;
}

void ServerSetup(void)
{
    // server_info.buffer_size = 0x10000;   // 64 Kb
    // server_info.max_iocp_threads = GetCurrentProcessorNumber();
    // server_info.max_capacity = 5000;
    // server_info.default_client_list_size = 100;
    bSetup = true;
}

bool ServerInit(void)
{
    if (!bSetup) {
        ServerSetup();
    }

    // pServer = (struct server_st*) malloc(sizeof(struct server_st));
    // if (pServer == NULL) {
    //     return false;
    // }

    // pServer->mem = (void*) malloc(sizeof(byte) * server_info.max_capacity * server_info.buffer_size);
    // if (pServer->mem == NULL) {
    //     safe_release(pServer);
    //     return false;
    // }
    // pServer->client_list.resize(server_info.default_client_list_size);          // set enough std::vector size
    // pServer->isRun = false;
    // pServer->listen_fd = NULL;
    return 0;
}

bool ServerDestruct(void)
{
    // if (!bSetup) {
    //     return false;
    // }
    // safe_release(pServer->mem);
    // safe_release(pServer);
    return true;
}

bool ServerRun(void)
{
    return 0;
}

bool ServerStop(void)
{
    return 0;
}

bool ServerRunNode(unsigned int index)
{
    return 0;
}

bool ServerStopNode(unsigned int index)
{
    return 0;
}

bool ServerRemoveNode(unsigned int index)
{
    return 0;
}

bool ServerSend(unsigned int index, char* buf)
{
    return 0;
}

bool ServerRecv(unsigned int index, char* buf)
{
    return 0;
}

void ServerSendAsync(unsigned int index, char* buf)
{
}

void ServerRecvAsync(unsigned int index, char* buf)
{
}

// Internal functions
void Add_node()
{

}

void Remove_node()
{

}
//
//Server_cl::Server_cl() : ready(false), m_capacity(0)
//{
//    m_memory = (char*)malloc(sizeof(char) * MAX_CAPACITY * BUFFER_SIZE);            // check free #1
//    m_nodes = (node_st*)malloc(sizeof(node_st) * MAX_CAPACITY);                     // check free #2
//}
//
//Server_cl::~Server_cl()
//{
//    Close_all_socket();
//
//    shutdown(m_hListen_socket, SD_BOTH);
//    closesocket(m_hListen_socket);
//    CloseHandle(m_hListen_thread);
//
//    for (int i = 0; i < MAX_IOCP_THREADS; i++) {
//        CloseHandle(m_phIocp_threads[i]);
//    }
//    free(m_phIocp_threads);
//
//    CloseHandle(m_hIocp);
//    DeleteCriticalSection(&m_cs);
//
//    free(m_nodes);      // check free #2
//    free(m_memory);     // check free #1
//}
//
//bool Server_cl::Setup()
//{
//    return true;
//}
//
//bool Server_cl::Init()
//{
//    for (unsigned int i = 0; i < MAX_CAPACITY; i++) {
//        m_nodes[i].socket = NULL;
//        m_nodes[i].buffer = &m_memory[i * BUFFER_SIZE];
//    }
//
//    ZeroMemory(&m_wsa, sizeof(WSADATA));
//    if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0) {
//        puts("Cannot Initialize winsock.");
//    }
//    
//    InitializeCriticalSection(&m_cs);
//
//    // set IOCP
//    m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
//    if (m_hIocp == NULL) {
//        puts("Cannot create iocp.");
//    }
//
//    m_phIocp_threads = (HANDLE*)malloc(sizeof(HANDLE) * MAX_IOCP_THREADS);
//    for (int i = 0; i < MAX_IOCP_THREADS; i++) {
//        m_phIocp_threads[i] = CreateThread(NULL, 0, ServerIocpThreadLoop, (LPVOID)this, 0, NULL);
//    }
//
//    // Set Listen Socket
//    m_hListen_socket = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
//
//    SOCKADDR_IN addrsvr;
//    addrsvr.sin_family = AF_INET;
//    addrsvr.sin_addr.S_un.S_addr = ::htonl(INADDR_ANY);
//    addrsvr.sin_port = ::htons(PORT);
//
//    if (bind(m_hListen_socket, (SOCKADDR*)&addrsvr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
//        puts("Cannot bind listen socket");
//    }
//
//    if (listen(m_hListen_socket, SOMAXCONN) == SOCKET_ERROR) {
//        puts("Cannot listen");
//    }
//
//    m_hListen_thread = CreateThread(NULL, 0, ServerListenThreadLoop, (LPVOID)this, 0, NULL);
//    //m_hListen_thread.join();
//    return true;
//}
//void Server_cl::Add_node(SOCKET hClient)
//{
//    int index = Find_seat();
//    if (index < 0) {
//        index = m_capacity;
//    }
//    m_nodes[index].stop = false;
//    m_nodes[index].occupied = true;
//    m_nodes[index].index = index;
//    m_nodes[index].socket = hClient;
//
//    // Connect socket to iocp
//    CreateIoCompletionPort((HANDLE)hClient, m_hIocp, (ULONG_PTR)&m_nodes[index], 0);
//    WSABUF wsaBuf;
//    wsaBuf.buf = m_nodes[index].buffer;
//    wsaBuf.len = BUFFER_SIZE;
//
//    DWORD nRecv;
//    DWORD flag = 0;
//    // Receive with async
//    WSARecv(hClient, &wsaBuf, 1, &nRecv, &flag, NULL, NULL);
//    if (WSAGetLastError() != WSA_IO_PENDING) {
//        puts("ERROR: WSARecv() != WSA_IO_PENDING");
//    }
//    m_capacity++;
//}
//
//void Server_cl::Remove_node(int index)
//{
//    Close_socket(m_nodes[index].socket);
//    m_nodes[index].stop = true;
//    m_nodes[index].occupied = false;
//    m_nodes[index].index = unsigned int16_MAX;
//    m_nodes[index].socket = NULL;
//    m_capacity--;
//}
//
//// helper function
//unsigned int32 Server_cl::Find_seat()
//{
//    // Return : #node_block if it has the seat.
//    //          -1 if there is not enough seat.
//    int ret = -1;
//    if (seats_list.size() == 0) {
//        return ret;
//    } else {
//        ret = seats_list.front();
//        seats_list.pop_front();
//        return ret;
//    }
//}
//
//// TODO: Is it need time for closing socket?
//void Server_cl::Close_socket(SOCKET socket)
//{
//    shutdown(socket, SD_BOTH);
//    closesocket(socket);
//}
//
//void Server_cl::Close_all_socket(void)
//{
//    for (unsigned int i = 0; i < m_capacity; i++)
//    {
//        if (m_nodes[i].occupied)
//        {
//            shutdown(m_nodes[i].socket, SD_BOTH);
//            closesocket(m_nodes[i].socket);
//        }
//    }
//}
//
//void Server_cl::Stop_node(unsigned int32 index)
//{
//    m_nodes[index].stop = true;
//}
//
//void Server_cl::Start_node(unsigned int32 index)
//{
//    m_nodes[index].stop = false;
//}
//
//
//
//// Server Thread Functions
//DWORD WINAPI ServerListenThreadLoop(LPVOID pParam)
//{

//}
//
//// Server Thread functions
//DWORD WINAPI ServerIocpThreadLoop(LPVOID pParam)
//{
//    Server_cl* p_server = (Server_cl*)pParam;
//    return 0;
//}
//
