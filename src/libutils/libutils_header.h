#include "libutils.h"

#include <thread>
#include <queue>
#include <mutex>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <system_error>

#ifndef LIBUTILS_HEADER_H_
#define LIBUTILS_HEADER_H_

#ifdef LINUX
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>
#else   // Windows
    #include <Windows.h>
    #include <tchar.h>
    HANDLE h_event_log;

#endif  // --- end --- Windows

#endif  // end - LIBUTILS_HEADER_H_

// ipc variables
struct _ipc_variables_t {
    std::mutex* mutex;
    int         n_users;
};

// internal functions
int shared_memory_init(void);
int global_variable_init(void);
int log_thread_init(void);
void thread_function_log(void);

