#include "libmath_header.h"
extern struct _dev_data dev_data;

bool DeviceReset(void)
{
    cudaError_t ret = cudaDeviceReset();
    return (ret == cudaSuccess) ? true : false;
}

bool addWithCuda(int* c, const int* a, const int* b, unsigned int size)
{
    cudaError_t ret = _addWithCuda(c, a, b, size);

    if (ret != cudaSuccess) {
        return false;
    }

    return true;
}

bool addWithCudaBegin(int* out, const int* input_a, const int* input_b, unsigned int size)
{
    if (g_dev_data_int32.isContinue) {
        return false;
    }

    SetDevice(0);
    g_dev_data_int32.isContinue = true;
    g_dev_data_int32.out = out;
    g_dev_data_int32.queue[0] = input_a;
    g_dev_data_int32.queue[1] = input_a;
    g_dev_data_int32.queue_size = 2;
    return true;
}

bool addWithCudaContinue(const int* input)
{
    if (!g_dev_data_int32.isContinue) {
        // not initialized
        return false;
    }
    if (g_dev_data_int32.queue_size >= g_max_queue_size.uint32) {
        // reached maximum queue size
        return false;
    }

    g_dev_data_int32.queue[g_dev_data_int32.queue_size] = input;
    g_dev_data_int32.queue_size++;
    return true;
}
bool addWithCudaTerminate(void)
{
    if (!g_dev_data_int32.isContinue) {
        // not initialized
        strcpy(g_error_message, "Not initialized.");
        return false;
    }
    cudaError_t ret = _addWithCudaQueue();

    g_dev_data_int32.isContinue = false;
    g_dev_data_int32.size = 0;
    g_dev_data_int32.out = 0;
    g_dev_data_int32.queue_size = 0;
    memset(g_dev_data_int32.queue, 0, sizeof(int*)* g_dev_data_int32.queue_size);
    
    if (ret != cudaSuccess) {
        strcpy(g_error_message, "Other errors.");
        return false;
    }
    return true;
}
