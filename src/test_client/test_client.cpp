#ifdef LINUX
#include <unistd.h>
#else
#ifdef DEBUG
#define safe_release(p) if(p) free(p)
#include <winsock2.h>
#pragma comment(lib, "ws2_32")
#include <windows.h>
// #pragma warning(suppress: 4996) // specific line
#pragma warning(disable: 4996) // time structure
#endif
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>


int main(int argc, char** argv)
{
 
    SOCKET g_listen = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    return 0;
}