#include "libfixprotocol.h"
//#include "libmath.h"
#include "libnetwork.h"
#include "libsql.h"
#include "libutils.h"

#ifdef LINUX
#include <unistd.h>
#else
// --- Windows ---


#ifdef DEBUG
// #pragma warning(suppress: 4996) // specific line
#pragma warning(disable: 4996) // time structure
#endif
// --- Windows ---
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>

//#include <thread>

int main(int argc, char** argv)
{
    HMODULE hModule = LoadLibrary(TEXT("libnetwork.dll"));
    if (hModule == NULL) {
        puts("Failed to read dll.");
        return 0;
    }
    unsigned int(__stdcall *ServerInit)(void);
    ServerInit = (unsigned int (__stdcall*)(void))GetProcAddress(hModule, "ServerInit");
   
    void(__stdcall * ServerDestruct)(void);
    ServerDestruct = (void(__stdcall*)(void))GetProcAddress(hModule, "ServerDestruct");


    ServerInit();
    ServerDestruct();

    
    return 0;
}