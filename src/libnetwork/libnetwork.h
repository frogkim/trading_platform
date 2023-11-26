#ifndef LIBNETWORK_H_
#define LIBNETWORK_H_

#ifdef LINUX
#ifdef EXPORT
#define LIBNETWORK __attribute__((visibility("default")))
#else
#define LIBNETWORK
#endif

#else  // Windows
#ifdef EXPORT
#define LIBNETWORK __declspec(dllexport)
#else
#define LIBNETWORK __declspec(dllimport)
#endif

#endif

// Server functions
extern "C" 
{
    LIBNETWORK void ServerSetup(unsigned int buf_size, unsigned int max_iocp_threads, unsigned int max_server_capacity, unsigned int default_client_list_size);
    LIBNETWORK bool ServerInit(void);
    LIBNETWORK bool ServerDestruct(void);
    LIBNETWORK bool ServerRun(void);
    LIBNETWORK bool ServerStop(void);
    LIBNETWORK bool ServerRunNode(unsigned int index);
    LIBNETWORK bool ServerStopNode(unsigned int index);
    LIBNETWORK bool ServerRemoveNode(unsigned int index);
    LIBNETWORK bool ServerSend(unsigned int index, char* buf);
    LIBNETWORK bool ServerRecv(unsigned int index, char* buf);
    LIBNETWORK void ServerSendAsync(unsigned int index, char* buf);
    LIBNETWORK void ServerRecvAsync(unsigned int index, char* buf);
}

// Client functions
extern "C"
{
    LIBNETWORK void ClientSetup(void);
    LIBNETWORK bool ClientInit(void);
    LIBNETWORK bool ClientDestruct(void);
    LIBNETWORK bool ClientConnect(void);
    LIBNETWORK bool ClientDisconnect(void);
    LIBNETWORK bool ClientStop(void);
    LIBNETWORK bool ClientSend(unsigned int index, char* buf);
    LIBNETWORK bool ClientRecv(unsigned int index, char* buf);
    LIBNETWORK void ClientSendAsync(unsigned int index, char* buf);
    LIBNETWORK void ClientRecvAsync(unsigned int index, char* buf);
}

#endif // end LIBNETWORK_H_
