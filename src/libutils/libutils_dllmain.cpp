#include "libutils_header.h"
#ifdef LINUX
// --- end --- LINUX
#else   
// --- start --- WINDOWS
BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        libutils_init();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        libutils_deinit();
        break;
    }
    return TRUE;
}


// --- end --- WINDOWS
#endif




#ifdef DEBUG

// common

// --- end --- common


#endif

