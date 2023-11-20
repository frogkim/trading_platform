#ifndef TRADING_PLATFORM_NETWORK_H_
#define TRADING_PLATFORM_NETWORK_H_

#ifdef _WINDLL
#define DllDecl __declspec(dllexport)
#else
#define DllDecl __declspec(dllimport)
#endif

#include <winSock2.h>
#pragma (lib, "ws2_32.lib")
#include <ws2tcpip.h>
#include <Windows.h>


#endif
