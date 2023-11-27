#ifndef LIBUTILS_H_
#define LIBUTILS_H_

#ifdef LINUX
#ifdef EXPORT
#define LIBUTILS __attribute__((visibility("default")))
#else
#define LIBUTILS
#endif

#else   // windows
#ifdef EXPORT
#define LIBUTILS __declspec(dllexport)
#else
#define LIBUTILS __declspec(dllimport)
#endif
#endif

extern "C"
{
    LIBUTILS int libutils_init(void);
    LIBUTILS int libutils_deinit(void);
    LIBUTILS void libutils_write_log(const char* message_c, unsigned int pid, void* time_spec);

#ifdef DEBUG
    #ifdef LINUX
    // --- end --- LINUX
    #else   
    // --- start --- WINDOWS
    #pragma warning(disable: 4996) // time structure warning
    // --- end --- WINDOWS
    #endif
#endif
}
#endif // end LIBUTILS_H_
