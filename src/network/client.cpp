#include "network_header.h"
static struct client_info_st client_info;
static struct client_st* pClient;

void ClientSetup(void)
{
    //char ip[16];
    //client_info_st() {
    //    ZeroMemory(ip, 16);
    //}
    //UINT buffer_size = 0x10000;   // 64 Kb
    //UINT MAX_IOCP_THREADS = 1;
}

bool ClientInit(void)
{
    pClient = (struct client_st*)malloc(sizeof(struct client_st));
    if (pClient == NULL) {
        return false;
    }

    pClient->buffer = (char*)malloc(sizeof(char) * client_info.buffer_size);
    if (pClient->buffer == NULL) {
        safe_release(pClient);
        return false;
    }




    pClient->isRun = false;
    pClient->socket = NULL;
    return true;
}

bool ClientDestruct(void)
{
    safe_release(pClient->buffer);
    safe_release(pClient);
    return 0;
}

bool ClientConnect(void)
{
    return 0;
}
bool ClientDisconnect(void)
{
    return 0;
}
bool ClientStop(void)
{
    return 0;
}
bool ClientSend(UINT index, char* buf)
{
    return 0;
}
bool ClientRecv(UINT index, char* buf)
{
    return 0;
}
void ClientSendAsync(UINT index, char* buf)
{
}
void ClientRecvAsync(UINT index, char* buf)
{
}