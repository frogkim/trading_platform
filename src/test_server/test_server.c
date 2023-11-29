#include <windows.h>
#include <tchar.h>
#include <stdio.h>

//
// Thread pool wait callback function template
//
VOID
CALLBACK
MyWaitCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_WAIT              Wait,
    TP_WAIT_RESULT        WaitResult
)
{
    // Instance, Parameter, Wait, and WaitResult not used in this example.
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(Parameter);
    UNREFERENCED_PARAMETER(Wait);
    UNREFERENCED_PARAMETER(WaitResult);

    //
    // Do something when the wait is over.
    //
    _tprintf(_T("MyWaitCallback: wait is over.\n"));
}


//
// Thread pool timer callback function template
//
VOID
CALLBACK
MyTimerCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_TIMER             Timer
)
{
    // Instance, Parameter, and Timer not used in this example.
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(Parameter);
    UNREFERENCED_PARAMETER(Timer);

    //
    // Do something when the timer fires.
    //
    _tprintf(_T("MyTimerCallback: timer has fired.\n"));

}


//
// This is the thread pool work callback function.
//
VOID
CALLBACK
MyWorkCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_WORK              Work
)
{
    // Instance, Parameter, and Work not used in this example.
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(Parameter);
    UNREFERENCED_PARAMETER(Work);

    BOOL bRet = FALSE;

    //
    // Do something when the work callback is invoked.
    //
    {
        _tprintf(_T("MyWorkCallback: Task performed.\n"));
    }

    return;
}

VOID
DemoCleanupPersistentWorkTimer()
{
    BOOL bRet = FALSE;
    PTP_WORK work = NULL;
    PTP_TIMER timer = NULL;
    PTP_POOL pool = NULL;
    PTP_WORK_CALLBACK workcallback = MyWorkCallback;
    PTP_TIMER_CALLBACK timercallback = MyTimerCallback;
    TP_CALLBACK_ENVIRON CallBackEnviron;
    PTP_CLEANUP_GROUP cleanupgroup = NULL;
    FILETIME FileDueTime;
    ULARGE_INTEGER ulDueTime;
    UINT rollback = 0;

    InitializeThreadpoolEnvironment(&CallBackEnviron);

    //
    // Create a custom, dedicated thread pool.
    //
    pool = CreateThreadpool(NULL);

    if (NULL == pool) {
        _tprintf(_T("CreateThreadpool failed. LastError: %u\n"),
            GetLastError());
        goto main_cleanup;
    }

    rollback = 1; // pool creation succeeded

    //
    // The thread pool is made persistent simply by setting
    // both the minimum and maximum threads to 1.
    //
    SetThreadpoolThreadMaximum(pool, 1);

    bRet = SetThreadpoolThreadMinimum(pool, 1);

    if (FALSE == bRet) {
        _tprintf(_T("SetThreadpoolThreadMinimum failed. LastError: %u\n"),
            GetLastError());
        goto main_cleanup;
    }

    //
    // Create a cleanup group for this thread pool.
    //
    cleanupgroup = CreateThreadpoolCleanupGroup();

    if (NULL == cleanupgroup) {
        _tprintf(_T("CreateThreadpoolCleanupGroup failed. LastError: %u\n"),
            GetLastError());
        goto main_cleanup;
    }

    rollback = 2;  // Cleanup group creation succeeded

    //
    // Associate the callback environment with our thread pool.
    //
    SetThreadpoolCallbackPool(&CallBackEnviron, pool);

    //
    // Associate the cleanup group with our thread pool.
    // Objects created with the same callback environment
    // as the cleanup group become members of the cleanup group.
    //
    SetThreadpoolCallbackCleanupGroup(&CallBackEnviron,
        cleanupgroup,
        NULL);

    //
    // Create work with the callback environment.
    //
    work = CreateThreadpoolWork(workcallback,
        NULL,
        &CallBackEnviron);

    if (NULL == work) {
        _tprintf(_T("CreateThreadpoolWork failed. LastError: %u\n"),
            GetLastError());
        goto main_cleanup;
    }

    rollback = 3;  // Creation of work succeeded

    //
    // Submit the work to the pool. Because this was a pre-allocated
    // work item (using CreateThreadpoolWork), it is guaranteed to execute.
    //
    SubmitThreadpoolWork(work);


    //
    // Create a timer with the same callback environment.
    //
    timer = CreateThreadpoolTimer(timercallback,
        NULL,
        &CallBackEnviron);


    if (NULL == timer) {
        _tprintf(_T("CreateThreadpoolTimer failed. LastError: %u\n"),
            GetLastError());
        goto main_cleanup;
    }

    rollback = 4;  // Timer creation succeeded

    //
    // Set the timer to fire in one second.
    //
    ulDueTime.QuadPart = (ULONGLONG)-(1 * 10 * 1000 * 1000);
    FileDueTime.dwHighDateTime = ulDueTime.HighPart;
    FileDueTime.dwLowDateTime = ulDueTime.LowPart;

    SetThreadpoolTimer(timer,
        &FileDueTime,
        0,
        0);

    //
    // Delay for the timer to be fired
    //
    Sleep(1500);

    //
    // Wait for all callbacks to finish.
    // CloseThreadpoolCleanupGroupMembers also releases objects
    // that are members of the cleanup group, so it is not necessary 
    // to call close functions on individual objects 
    // after calling CloseThreadpoolCleanupGroupMembers.
    //
    CloseThreadpoolCleanupGroupMembers(cleanupgroup,
        FALSE,
        NULL);

    //
    // Already cleaned up the work item with the
    // CloseThreadpoolCleanupGroupMembers, so set rollback to 2.
    //
    rollback = 2;
    goto main_cleanup;

main_cleanup:
    //
    // Clean up any individual pieces manually
    // Notice the fall-through structure of the switch.
    // Clean up in reverse order.
    //

    switch (rollback) {
    case 4:
    case 3:
        // Clean up the cleanup group members.
        CloseThreadpoolCleanupGroupMembers(cleanupgroup,
            FALSE, NULL);
    case 2:
        // Clean up the cleanup group.
        CloseThreadpoolCleanupGroup(cleanupgroup);

    case 1:
        // Clean up the pool.
        CloseThreadpool(pool);

    default:
        break;
    }

    return;
}

VOID
DemoNewRegisterWait()
{
    PTP_WAIT Wait = NULL;
    PTP_WAIT_CALLBACK waitcallback = MyWaitCallback;
    HANDLE hEvent = NULL;
    UINT i = 0;
    UINT rollback = 0;

    //
    // Create an auto-reset event.
    //
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if (NULL == hEvent) {
        // Error Handling
        return;
    }

    rollback = 1; // CreateEvent succeeded

    Wait = CreateThreadpoolWait(waitcallback,
        NULL,
        NULL);

    if (NULL == Wait) {
        _tprintf(_T("CreateThreadpoolWait failed. LastError: %u\n"),
            GetLastError());
        goto new_wait_cleanup;
    }

    rollback = 2; // CreateThreadpoolWait succeeded

    //
    // Need to re-register the event with the wait object
    // each time before signaling the event to trigger the wait callback.
    //
    for (i = 0; i < 5; i++) {
        SetThreadpoolWait(Wait,
            hEvent,
            NULL);

        SetEvent(hEvent);

        //
        // Delay for the waiter thread to act if necessary.
        //
        Sleep(500);

        //
        // Block here until the callback function is done executing.
        //

        WaitForThreadpoolWaitCallbacks(Wait, FALSE);
    }

new_wait_cleanup:
    switch (rollback) {
    case 2:
        // Unregister the wait by setting the event to NULL.
        SetThreadpoolWait(Wait, NULL, NULL);

        // Close the wait.
        CloseThreadpoolWait(Wait);

    case 1:
        // Close the event.
        CloseHandle(hEvent);

    default:
        break;
    }
    return;
}

int main(void)
{
    DemoNewRegisterWait();
    DemoCleanupPersistentWorkTimer();
    return 0;
}



// use fixed size queue
//#include <stdio.h>
//#include <stdint.h>
//#include <stdlib.h>
//#define bool uint32_t
//#define true 1
//#define false 0
//
//typedef struct _queue_manager_st {
//    uint32_t first;
//    uint32_t last;
//    uint32_t capacity;
//    uint32_t size;
//    bool (*set_seat)(struct _queue_manager_st*, int32_t);
//    int32_t(*get_seat)(struct _queue_manager_st*);
//    int32_t* seats;
//} queue_manager_st, *p_queue_manager_st;
//
//bool _set_seat(p_queue_manager_st, int32_t);
//int32_t _get_seat(p_queue_manager_st);
//
//
//void init(p_queue_manager_st this, uint32_t capacity)
//{
//    this->first = 0;
//    this->last = 0;
//    this->capacity = capacity;
//    this->size = 0;
//    this->set_seat = _set_seat;
//    this->get_seat = _get_seat;
//    this->seats = (int32_t*)malloc(sizeof(int32_t) * this->capacity);
//}
//
//bool _set_seat(p_queue_manager_st this, int32_t seat)
//{
//    if (this->size == this->capacity) {
//        return false;
//    }
//
//    this->seats[this->last] = seat;
//    this->last++;
//    this->size++;
//
//    if (this->last == this->capacity) {
//        this->last = 0;
//    }
//    return true;
//}
//
//int32_t _get_seat(p_queue_manager_st this)
//{
//    if (this->size == 0) {
//        return this->capacity;
//    }
//    int32_t ret = this->seats[this->first];
//    this->first++;
//    this->size--;
//    if (this->first == this->capacity) {
//        this->first = 0;
//    }
//    return ret;
//}
//
//void deinit(p_queue_manager_st this, uint32_t capacity)
//{
//    free(this->seats);
//}
//
//int main()
//{
//    queue_manager_st m;
//    init(&m, 5000);
//    uint32_t seat;
//    bool ret;
//
//    for (int i = 0; i < 2501; i++) {
//        ret = m.set_seat(&m, i);
//        if (!ret) {
//            printf("Failed to set seat %d\n", i);
//        } 
//    }
//
//    for (int i = 0; i < 2502; i++) {
//        seat = m.get_seat(&m);
//        if (seat == m.capacity) {
//            printf("Failed to get seat. %d\n", i);
//        }
//    }
//
//    for (int i = 0; i < 5003; i++) {
//        ret = m.set_seat(&m, i);
//        if (!ret) {
//            printf("Failed to set seat %d\n", i);
//        }
//    }
//
//    for (int i = 0; i < 5002; i++) {
//        seat = m.get_seat(&m);
//        if (seat == m.capacity) {
//            printf("Failed to get seat.%d\n", i);
//        }
//    }
//
//    return 0;
//}

// allocate memory each node
//typedef struct _seat_queue_st {
//    uint32_t seat;
//    struct _seat_queue_st* next;
//} seat_queue_st, *p_seat_queue_st;
//
//typedef struct _queue_manager {
//    struct _queue_manager* this;
//    uint32_t size;
//    uint32_t capacity;
//    p_seat_queue_st prior_seat;
//    p_seat_queue_st last_seat;
//    bool (*set_seat)(struct _queue_manager*, uint32_t);
//    uint32_t(*get_seat)(struct _queue_manager*);
//} queue_manager_st, * p_queue_manager_st;
//
//bool _set_seat(p_queue_manager_st this, uint32_t capacity_i);
//uint32_t _get_seat(p_queue_manager_st);
//
//void init(p_queue_manager_st qm, uint32_t capacity_i)
//{
//    qm->this = qm;
//    qm->size = 0;
//    qm->capacity = capacity_i;
//    qm->prior_seat = NULL;
//    qm->last_seat = NULL;
//    qm->set_seat = _set_seat;
//    qm->get_seat = _get_seat;
//}
//
//bool _set_seat(p_queue_manager_st this, uint32_t i)
//{
//    if (this->last_seat == NULL) {
//        if (this->prior_seat == NULL) {
//            this->prior_seat = (p_seat_queue_st)malloc(sizeof(seat_queue_st));
//            this->prior_seat->seat = i;
//            this->prior_seat->next = NULL;
//        } else {
//            this->prior_seat->next = (p_seat_queue_st)malloc(sizeof(seat_queue_st));
//            this->last_seat =this->prior_seat->next;
//            this->last_seat->seat = i;
//            this->last_seat->next = NULL;
//        } 
//    } else {
//        this->last_seat->next = (p_seat_queue_st)malloc(sizeof(seat_queue_st));
//        this->last_seat = this->last_seat->next;
//        this->last_seat->seat = i;
//        this->last_seat->next = NULL;
//    }
//    this->size++;
//    return true;
//}
//
//uint32_t _get_seat(p_queue_manager_st this)
//{
//    if (this->size == this->capacity || this->prior_seat == NULL) {
//        return this->capacity;
//    }
//    uint32_t ret = this->prior_seat->seat;
//    p_seat_queue_st tmp = this->prior_seat->next;
//    free(this->prior_seat);
//    this->prior_seat = tmp;
//    if (this->prior_seat == this->last_seat) {
//        this->last_seat = NULL;
//    } 
//    this->size--;
//    return ret;
//}
//
//
//int main()
//{
//    queue_manager_st m;
//    init(&m, 500);
//    uint32_t seat;
//    bool ret;
//    
//    seat = m.get_seat(&m);
//    printf("get_seat result: %d\n", seat);
//
//    for (int i = 0; i < 5; i++) {
//        ret = m.set_seat(&m, i);
//        if (ret) {
//            printf("set seat %d successed\n", i);
//        } else {
//            printf("Failed to set seat %d\n", i);
//        }
//    }
//
//    for (int i = 0; i < 7; i++) {
//        seat = m.get_seat(&m);
//        if (seat == m.capacity) {
//            printf("Failed to get seat.\n", i);
//        } else {
//            printf("get_seat result: %d\n", seat);
//        }
//    }
//
//
//    for (int i = 0; i < 5; i++) {
//        ret = m.set_seat(&m, i);
//        if (ret) {
//            printf("set seat %d successed\n", i);
//        } else {
//            printf("Failed to set seat %d\n", i);
//        }
//    }
//
//    for (int i = 0; i < 7; i++) {
//        seat = m.get_seat(&m);
//        if (seat == m.capacity) {
//            printf("Failed to get seat.\n", i);
//        } else {
//            printf("get_seat result: %d\n", seat);
//        }
//    }
//
//
//    return 0;
//}