#ifndef LIBNETWORK_H_
#define LIBNETWORK_H_

#ifdef LINUX
#ifdef EXPORT
#define LIBNETWORK __attribute__((visibility("default")))
#else
#define LIBNETWORK
#endif

// Server functions
extern LIBNETWORK void __stdcall ServerSetup(unsigned int buf_size, unsigned int max_iocp_threads, unsigned int max_server_capacity, unsigned int default_client_list_size);
extern LIBNETWORK BOOL __stdcall ServerInit(void);
extern LIBNETWORK BOOL __stdcall ServerDestruct(void);
extern LIBNETWORK BOOL __stdcall ServerRun(void);
extern LIBNETWORK BOOL __stdcall ServerStop(void);
extern LIBNETWORK BOOL __stdcall ServerRunNode(unsigned int index);
extern LIBNETWORK BOOL __stdcall ServerStopNode(unsigned int index);
extern LIBNETWORK BOOL __stdcall ServerRemoveNode(unsigned int index);
extern LIBNETWORK BOOL __stdcall ServerSend(unsigned int index, char* buf);
extern LIBNETWORK BOOL __stdcall ServerRecv(unsigned int index, char* buf);
extern LIBNETWORK void __stdcall ServerSendAsync(unsigned int index, char* buf);
extern LIBNETWORK void __stdcall ServerRecvAsync(unsigned int index, char* buf);

// Client functions
extern LIBNETWORK void __stdcall ClientSetup(void);
extern LIBNETWORK BOOL __stdcall ClientInit(void);
extern LIBNETWORK BOOL __stdcall ClientDestruct(void);
extern LIBNETWORK BOOL __stdcall ClientConnect(void);
extern LIBNETWORK BOOL __stdcall ClientDisconnect(void);
extern LIBNETWORK BOOL __stdcall ClientStop(void);
extern LIBNETWORK BOOL __stdcall ClientSend(char* buf);
extern LIBNETWORK BOOL __stdcall ClientRecv(char* buf);
extern LIBNETWORK void __stdcall ClientSendAsync(char* buf);
extern LIBNETWORK void __stdcall ClientRecvAsync(char* buf);

#else  // --------------------------------- Windows ---------------------------------
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#include <windows.h>

#ifdef EXPORT
#define LIBNETWORK __declspec(dllexport)
#else
#define LIBNETWORK __declspec(dllimport)
#endif

#endif

// Server functions
extern LIBNETWORK unsigned int __stdcall ServerInit(void);
extern LIBNETWORK void __stdcall ServerDestruct(int step);
extern LIBNETWORK void __stdcall ServerRun(void);
extern LIBNETWORK void __stdcall ServerStop(void);
extern LIBNETWORK unsigned int __stdcall ServerRunNode(unsigned int index);
extern LIBNETWORK unsigned int __stdcall ServerStopNode(unsigned int index);
extern LIBNETWORK unsigned int __stdcall ServerRemoveNode(unsigned int index);
extern LIBNETWORK unsigned int __stdcall ServerSend(unsigned int index, char* buf);
extern LIBNETWORK unsigned int __stdcall ServerRecv(unsigned int index, char* buf);
extern LIBNETWORK void __stdcall ServerSendAsync(unsigned int index, char* buf);
extern LIBNETWORK void __stdcall ServerRecvAsync(unsigned int index, char* buf);

extern LIBNETWORK void __stdcall ServerSetBufferSize(unsigned int value);
extern LIBNETWORK void __stdcall ServerSetCapacity(unsigned int value);
extern LIBNETWORK void __stdcall ServerSetNumberIocpThreads(unsigned int value);
extern LIBNETWORK void __stdcall ServerSetNumberIocpPortThreads(unsigned int value);
// Client functions
extern LIBNETWORK unsigned int __stdcall ClientInit(void);
extern LIBNETWORK unsigned int __stdcall ClientDestruct(void);
extern LIBNETWORK unsigned int __stdcall ClientConnect(void);
extern LIBNETWORK unsigned int __stdcall ClientDisconnect(void);
extern LIBNETWORK unsigned int __stdcall ClientStop(void);
extern LIBNETWORK unsigned int __stdcall ClientSend(char* buf);
extern LIBNETWORK unsigned int __stdcall ClientRecv(char* buf);
extern LIBNETWORK void __stdcall ClientSendAsync(char* buf);
extern LIBNETWORK void __stdcall ClientRecvAsync(char* buf);
       // --------------------------------- Windows ---------------------------------
#endif // end LIBNETWORK_H_
