#ifndef LIBUTILS_HEADER_H_
#define LIBUTILS_HEADER_H_
#include "libutils.h"

#include <fstream>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <string>
#include <chrono>
#include <ctime>
#include <system_error>

// includes
#ifdef LINUX
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>
#else   // Windows
    #include <Windows.h>
    #include <tchar.h>
#endif  // --- end --- Windows

// ipc variables
typedef struct _ipc_variables_t {
    std::mutex*     mutex;
    int             n_users;
}ipc_variables_t, *p_ipc_variables_t;

typedef struct _input_data {
    char                message[256];
    unsigned int        pid; 
    struct timespec     ts;
    _input_data(const char* m, unsigned int p, void* p_ts) : pid(p)
    {
        memcpy(message, m, sizeof(char)*256);
        memcpy(&ts, p_ts, sizeof(struct timespec));
    }
} input_data_t;

// internal functions
int shared_memory_init(void);
int global_variable_init(void);
int log_thread_init(void);
void thread_function_log(void);
void write_log(void);

// global variable
extern p_ipc_variables_t        g_p_ipc_variables;
extern std::thread              g_thread_log;
extern std::queue<input_data_t> g_deque_log;
extern std::mutex               g_mutex_log;
extern std::fstream             g_File;

#ifdef LINUX

// --- end --- LINUX
#else
// Windows
extern HANDLE                   evts[2];
// --- end --- Windows
#endif  
extern const unsigned int       size_shared_memory;
extern const char*              name_shared_memory;


#endif  // end - LIBUTILS_HEADER_H_