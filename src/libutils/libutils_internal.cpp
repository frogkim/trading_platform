#include "libutils_header.h"
#ifdef LINUX

// --- end --- LINUX
#else
// Windows

// --- end --- Windows
#endif  

// internal functions
int libutils_init(void)
{
    int ret;
    ret = shared_memory_init();
    if (ret) {
        return ret;
    }

    ret = global_variable_init();
    if (ret) {
        return ret;
    }

    ret = log_thread_init();
    if (ret) {
        return ret;
    }
    return ret;
}

int libutils_deinit(void)
{
    SetEvent(evts[0]);
    if (g_thread_log.joinable()) {
        g_thread_log.join();
    }
    CloseHandle(evts[0]);
    CloseHandle(evts[1]);
    if (g_p_ipc_variables == NULL) {
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

    if (g_p_ipc_variables->mutex != nullptr) {
        return -1;
    }
    return 0;
}

int shared_memory_init(void)
{
#ifdef LINUX
    // --- end --- LINUX
#else
    // Windows
    g_pid = GetCurrentProcessId();
    bool b_already_created = false;
    HANDLE h_shared_memory = (void*)CreateFileMapping(
        INVALID_HANDLE_VALUE,           // use paging file
        NULL,                           // default security
        PAGE_READWRITE,                 // read/write access
        0,                              // maximum object size (high-order DWORD)
        size_shared_memory,             // maximum object size (low-order DWORD)
        (TCHAR*)name_shared_memory);    // name of mapping object

    if (h_shared_memory == nullptr) {
        return GetLastError();
    }

    if (GetLastError() != ERROR_ALREADY_EXISTS) {
        b_already_created = true;
    }

    g_p_ipc_variables = (p_ipc_variables_t)MapViewOfFile(h_shared_memory,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        size_shared_memory);
    if (!b_already_created && g_p_ipc_variables == nullptr) {
        return GetLastError();
    }
    ZeroMemory(g_p_ipc_variables, sizeof(ipc_variables_t));
    CloseHandle(h_shared_memory);
    if (g_p_ipc_variables == nullptr) {
        return GetLastError();
    }
    // --- end --- Windows
#endif  

    return 0;
}

int global_variable_init(void)
{
    if (g_p_ipc_variables->mutex == nullptr) {
        g_p_ipc_variables->mutex = new std::mutex;
        g_p_ipc_variables->n_users = 0;
    }

    if (g_p_ipc_variables->mutex == nullptr) {
        return -1;
    }

    g_p_ipc_variables->mutex->lock();
    g_p_ipc_variables->n_users++;
    g_p_ipc_variables->mutex->unlock();

    evts[0] = CreateEvent(
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        TEXT("event_log_terminate") // object name
    );


    evts[1] = CreateEvent(
        NULL,               // default security attributes
        TRUE,               // manual-reset event
        FALSE,              // initial state is nonsignaled
        TEXT("event_log_write") // object name
    );

    if (evts[0] == nullptr || evts[1] == nullptr) {
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
        dwWaitResult = WaitForMultipleObjects(2, evts, false, INFINITE);
        if (dwWaitResult == WAIT_OBJECT_0) {    // evts[0] - end event
            break;
        }
        if (dwWaitResult == WAIT_OBJECT_0 + 1) { // evts[1] - write log event
            write_log();
        }
    } while (true);
}

void write_log(void)
{
    char buf[100];
    struct timespec ts;
    std::string input_message;
    std::fstream fs;
    while (g_deque_log.size() > 0)
    {
        // Use internal lock - retrieve from queue. 
        g_mutex_log.lock();
        input_data_t data = g_deque_log.front();
        g_deque_log.pop();
        g_mutex_log.unlock();

        // transform input time to string
        strftime(buf, sizeof(buf), "%D %T", gmtime(&(data.ts.tv_sec)));
        // get current time
        int ret = timespec_get(&ts, TIME_UTC);
        double diff = (double) (ts.tv_nsec - data.ts.tv_nsec) / 1000.0;
        
        input_message.clear();
        input_message += '[';
        input_message += buf;
        input_message += "] pid: ";
        std::sprintf(buf, "%4d", g_pid);
        input_message += buf;
        input_message += " - ";
        input_message += data.message;
        input_message += " Diff: ";
        std::sprintf(buf, "%.3f", diff);
        input_message += buf;
        input_message += " ms\n";
        
        g_p_ipc_variables->mutex->lock();
        fs.open("log.txt", std::fstream::out | std::fstream::app);
        fs << input_message;
        fs.close();
        g_p_ipc_variables->mutex->unlock();
    }
    ResetEvent(evts[1]);
}
