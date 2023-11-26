#include "libsql.h"

#ifndef LIBSQL_HEADER_H_
#define LIBSQL_HEADER_H_

#ifdef LINUX

#else   // Windows

#include <winSock2.h>
#pragma (lib, "ws2_32.lib")
#include <ws2tcpip.h>
#include <Windows.h>
#endif  // --- end --- Windows


#endif