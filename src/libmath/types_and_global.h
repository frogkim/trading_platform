#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

static cudaError_t g_last_cuda_error = cudaSuccess;
static char g_error_message[256] = { 0 };

static struct _max_queue_size {
    const uint32_t uint32 = 100;
} g_max_queue_size;

static struct _dev_data_int32 {
    bool isContinue;
    int size = sizeof(int32_t);
    int* out;
    const int* queue[100];
    uint32_t queue_size;
} g_dev_data_int32;
