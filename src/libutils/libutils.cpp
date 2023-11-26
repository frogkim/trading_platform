#include "libutils_header.h"

// ipc variable
struct _ipc_variables_t* g_p_ipc_variables_st;

// global variable
std::thread             g_thread_log;
std::queue<std::string> g_deque_log;
std::mutex              g_mutex_log;

const unsigned int      pointer_size = sizeof(void*);
const unsigned int      size_shared_memory = sizeof(struct _ipc_variables_t);
const char*             name_shared_memory = "libutils_shared_memory";
unsigned int*           n_users;

#ifdef LINUX
#else   // Windows
extern HANDLE h_event_log;
#endif  // --- end --- Windows

// export functions
int libutils_init(void)
{
    int ret;
    bool already_exist;

    ret = shared_memory_init();
    if (!ret) {
        return ret;
    }

    global_variable_init();

    ret = log_thread_init();
    if (!ret) {
        return ret;
    }

    return ret;
}

int libutils_deinit(void)
{
    CloseHandle(h_event_log);
    
    (*g_p_ipc_variables_st->mutex).lock();
    g_p_ipc_variables_st->n_users--;
    (*g_p_ipc_variables_st->mutex).unlock();

    // free ipc mutext
    if (g_p_ipc_variables_st->n_users == 0) {
        delete g_p_ipc_variables_st->mutex;
    }

    if(g_p_ipc_variables_st->mutex != nullptr) {
        return -1;
    }
    return 0;
}

void libutils_write_log(char* s)
{
    g_mutex_log.lock();
    g_deque_log.push(std::string(s));
    g_mutex_log.unlock();
#ifdef LINUX
#else   // Windows
    SetEvent(h_event_log);
#endif  // --- end --- Windows
}

// internal functions
int shared_memory_init(void)
{
#ifdef LINUX

#else   // Windows
    HANDLE h_shared_memory = (void*)CreateFileMapping(
        INVALID_HANDLE_VALUE,           // use paging file
        NULL,                           // default security
        PAGE_READWRITE,                 // read/write access
        0,                              // maximum object size (high-order DWORD)
        size_shared_memory,             // maximum object size (low-order DWORD)
        (TCHAR*)name_shared_memory);    // name of mapping object
    
    if (h_shared_memory == NULL) {
        return GetLastError();
    }

    g_p_ipc_variables_st = (struct _ipc_variables_t*) MapViewOfFile(h_shared_memory,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        size_shared_memory);

    if (g_p_ipc_variables_st == NULL) {
        CloseHandle(h_shared_memory);
        return GetLastError();
    }
#endif  // --- end --- Windows    

    return 0;
}

int global_variable_init(void)
{
    if (g_p_ipc_variables_st->n_users > 0) {
        g_p_ipc_variables_st->mutex = new std::mutex;
    }

    if (g_p_ipc_variables_st->mutex == nullptr) {
        return -1;
    }

    (*g_p_ipc_variables_st->mutex).lock();
    g_p_ipc_variables_st->n_users++;
    (*g_p_ipc_variables_st->mutex).unlock();

    h_event_log = CreateEvent(
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        TEXT("h_event_log") // object name
    );

    if (h_event_log == nullptr) {
        return -1;
    }

    return 0;
}

int log_thread_init(void)
{
    try {
        g_thread_log = std::thread(thread_function_log);
    }
    catch (std::system_error error_code) {
        if (error_code.code() == std::errc::resource_unavailable_try_again) {
            // known exception
            return 1;
        } else {
            // unknown exception
            return -1;
        }
    }
    catch (std::exception e) {
        // unknown exception
        return -1;
    }
    return 0;
}

void thread_function_log(void)
{
    DWORD dwWaitResult;
    
    do {
        dwWaitResult = WaitForSingleObjectEx(h_event_log, INFINITE, true);
        if (dwWaitResult != WAIT_OBJECT_0) {
            break;
        }
    } while(dwWaitResult);
}

void write_log(void)
{
    g_mutex_log.lock();
    (*g_p_ipc_variables_st->mutex).lock();
    while (g_deque_log.size())
    {
        std::string message = g_deque_log.front();
        g_deque_log.pop();
        // TODO: file write
    }
    (*g_p_ipc_variables_st->mutex).unlock();
    g_mutex_log.unlock();
}