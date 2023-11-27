#include "libutils_header.h"
// global variable
unsigned int             g_pid;
p_ipc_variables_t        g_p_ipc_variables;
std::thread              g_thread_log;
std::queue<input_data_t> g_deque_log;
std::mutex               g_mutex_log;

#ifdef LINUX

// --- end --- LINUX
#else
// Windows
HANDLE                   evts[2];
// --- end --- Windows
#endif  

// configuration
const unsigned int      size_shared_memory = sizeof(struct _ipc_variables_t);
const char*             name_shared_memory = "libutils_shared_memory";

// export functions

void libutils_write_log(const char* message_c, void* time_spec)
{
    g_mutex_log.lock();
    g_deque_log.push(input_data_t(message_c, time_spec));
    g_mutex_log.unlock();
#ifdef LINUX
    // --- end --- LINUX
#else
    // Windows
    SetEvent(evts[1]);
    // --- end --- Windows
#endif
}
