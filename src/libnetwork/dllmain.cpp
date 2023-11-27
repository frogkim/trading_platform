#include "libnetwork_header.h"


#ifdef LINUX
// Linux

// --- end --- Linux
#else
// Windows
bool WINAPI DllMain(HINSTANCE hinstDLL,  DWORD fdwReason, LPVOID lpvReserved)
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        break;

    case DLL_THREAD_ATTACH:
        break;

    case DLL_THREAD_DETACH:
        break;

    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}
#endif

