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
    LIBUTILS void libutils_write_log(char* s);
}
#endif // end LIBUTILS_H_
