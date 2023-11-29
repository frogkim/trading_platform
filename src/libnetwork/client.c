#include "libnetwork_header.h"

struct client_st {
    BOOL isRun;
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


void ClientSetup(void)
{
    //char ip[16];
    //client_info_st() {
    //    ZeroMemory(ip, 16);
    //}
    //unsigned int buffer_size = 0x10000;   // 64 Kb
    //unsigned int MAX_IOCP_THREADS = 1;
}

unsigned int ClientInit(void)
{
    // pClient = (struct client_st*)malloc(sizeof(struct client_st));
    // if (pClient == NULL) {
    //     return false;
    // }

    // pClient->buffer = (char*)malloc(sizeof(char) * client_info.buffer_size);
    // if (pClient->buffer == NULL) {
    //     safe_release(pClient);
    //     return false;
    // }

    // pClient->isRun = false;
    // pClient->socket = NULL;
    return TRUE;
}

unsigned int ClientDestruct(void)
{
    // safe_release(pClient->buffer);
    // safe_release(pClient);
    return 0;
}

unsigned int ClientConnect(void)
{
    return 0;
}
unsigned int ClientDisconnect(void)
{
    return 0;
}
unsigned int ClientStop(void)
{
    return 0;
}
unsigned int ClientSend(char* buf)
{
    return 0;
}
unsigned int ClientRecv(char* buf)
{
    return 0;
}
void ClientSendAsync(char* buf)
{
}
void ClientRecvAsync(char* buf)
{
}