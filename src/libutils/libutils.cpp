#include "libutils_header.h"
// global variable
p_ipc_variables_t        g_p_ipc_variables;
std::thread              g_thread_log;
std::queue<input_data_t> g_deque_log;
std::mutex               g_mutex_log;
std::fstream             g_File;

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
int libutils_init(void)
{
    int ret;
    ret = shared_memory_init();
    if (!ret) {
        return ret;
    }

    ret = global_variable_init();
    if (!ret) {
        return ret;
    }

    ret = log_thread_init();
    if (!ret) {
        return ret;
    }
    return ret;
}

int libutils_deinit(void)
{
    SetEvent(evts[0]);
    g_thread_log.join();
    CloseHandle(evts[0]);
    CloseHandle(evts[1]);
    if(g_p_ipc_variables == NULL) {
        return 2;
    }

    if (g_p_ipc_variables->mutex == nullptr) {
        return 1;
    }

    g_p_ipc_variables->mutex->lock();
    g_p_ipc_variables->n_users--;
    g_p_ipc_variables->mutex->unlock();

    // release ipc mutex
    if (g_p_ipc_variables->n_users == 0) {
        delete g_p_ipc_variables->mutex;
    }

    if(g_p_ipc_variables->mutex != nullptr) {
        return -1;
    }
    return 0;
}

void libutils_write_log(const char* message_c, unsigned int pid, void* time_spec)
{
    g_mutex_log.lock();
    g_deque_log.push(input_data_t(message_c, pid, time_spec));
    g_mutex_log.unlock();
#ifdef LINUX
    // --- end --- LINUX
#else
    // Windows
    SetEvent(evts[1]);
    // --- end --- Windows
#endif
}
