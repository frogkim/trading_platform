#ifndef TRADING_PLATFORM_NETWORK_H_
#define TRADING_PLATFORM_NETWORK_H_

#ifdef _WINDLL
    #define NETWORK_DECLSPEC __declspec(dllexport)
    #include <tchar.h>
    #include <winSock2.h>
    #pragma comment(lib, "ws2_32.lib")
    #include <ws2tcpip.h>
    #include <Windows.h>
#else
#define NETWORK_DECLSPEC __declspec(dllimport)
#endif

#include <ctype.h>

// Server functions
extern "C" 
{
    NETWORK_DECLSPEC void ServerSetup(UINT buf_size, UINT max_iocp_threads, UINT max_server_capacity, UINT default_client_list_size);
    NETWORK_DECLSPEC bool ServerInit(void);
    NETWORK_DECLSPEC bool ServerDestruct(void);
    NETWORK_DECLSPEC bool ServerRun(void);
    NETWORK_DECLSPEC bool ServerStopvoid();
    NETWORK_DECLSPEC bool ServerRunNode(UINT index);
    NETWORK_DECLSPEC bool ServerStopNode(UINT index);
    NETWORK_DECLSPEC bool ServerRemoveNode(UINT index);
    NETWORK_DECLSPEC bool ServerSend(UINT index, char* buf);
    NETWORK_DECLSPEC bool ServerRecv(UINT index, char* buf);
    NETWORK_DECLSPEC void ServerSendAsync(UINT index, char* buf);
    NETWORK_DECLSPEC void ServerRecvAsync(UINT index, char* buf);
}

// Client functions
extern "C"
{
    NETWORK_DECLSPEC void ClientSetup(void);
    NETWORK_DECLSPEC bool ClientInit(void);
    NETWORK_DECLSPEC bool ClientDestruct(void);
    NETWORK_DECLSPEC bool ClientConnect(void);
    NETWORK_DECLSPEC bool ClientDisconnect(void);
    NETWORK_DECLSPEC bool ClientStop(void);
    NETWORK_DECLSPEC bool ClientSend(UINT index, char* buf);
    NETWORK_DECLSPEC bool ClientRecv(UINT index, char* buf);
    NETWORK_DECLSPEC void ClientSendAsync(UINT index, char* buf);
    NETWORK_DECLSPEC void ClientRecvAsync(UINT index, char* buf);
}

#endif
