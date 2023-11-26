#include "libmath_header.h"
extern struct _dev_data dev_data;

bool isError(void)
{
    return g_last_cuda_error != cudaSuccess;
}

char* getLastErrsorMessage_pChar(void)
{
    return g_error_message;
}

bool SetDevice(int index = 0)
{
    // Choose which GPU to run on, change this on a multi-GPU system.
    g_last_cuda_error = cudaSetDevice(index);
    if (g_last_cuda_error != cudaSuccess) {
        strcpy(g_error_message, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
        return false;
    }
    return true;
}

__global__ void _addKernel(int *c, const int *a, const int *b)
{
    int i = threadIdx.x;
    c[i] = a[i] + b[i];
}

__global__ void _addWithQueueKernel(int* out, const int* queue)
{
    int i = threadIdx.x;
    out[i] = queue[0] + queue[1];
    
}

// Helper function for using CUDA to add vectors in parallel.
cudaError_t _addWithCuda(int* c, const int* a, const int* b, unsigned int size)
{
    int* dev_a = 0;
    int* dev_b = 0;
    int* dev_c = 0;
    cudaError cudaStatus;
    // Choose which GPU to run on, change this on a multi-GPU system.
    cudaStatus = cudaSetDevice(0);
    if (g_last_cuda_error != cudaSuccess) {
        strcpy(g_error_message, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
    }

    // Allocate GPU buffers for three vectors (two input, one output)    .
    cudaStatus = cudaMalloc((void**)&dev_c, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_a, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    cudaStatus = cudaMalloc((void**)&dev_b, size * sizeof(int));
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMalloc failed!");
        goto Error;
    }

    // Copy input vectors from host memory to GPU buffers.
    cudaStatus = cudaMemcpy(dev_a, a, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    cudaStatus = cudaMemcpy(dev_b, b, size * sizeof(int), cudaMemcpyHostToDevice);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

    // Launch a kernel on the GPU with one thread for each element.
    _addKernel <<<1, size >>> (dev_c, dev_a, dev_b);

    // Check for any errors launching the kernel
    cudaStatus = cudaGetLastError();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "addKernel launch failed: %s\n", cudaGetErrorString(cudaStatus));
        goto Error;
    }

    // cudaDeviceSynchronize waits for the kernel to finish, and returns
    // any errors encountered during the launch.
    cudaStatus = cudaDeviceSynchronize();
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaDeviceSynchronize returned error code %d after launching addKernel!\n", cudaStatus);
        goto Error;
    }

    // Copy output vector from GPU buffer to host memory.
    cudaStatus = cudaMemcpy(c, dev_c, size * sizeof(int), cudaMemcpyDeviceToHost);
    if (cudaStatus != cudaSuccess) {
        fprintf(stderr, "cudaMemcpy failed!");
        goto Error;
    }

Error:
    cudaFree(dev_c);
    cudaFree(dev_a);
    cudaFree(dev_b);

    return cudaStatus;
}


cudaError_t _addWithCudaQueue(void)
{
    cudaError cudaStatus = cudaSuccess;

    cudaStatus = cudaMalloc((void**)&g_dev_data_int32.out, sizeof(int) * sizeof(uint32_t));
    if (cudaStatus != cudaSuccess) {
        strcpy(g_error_message, "Failed to allocate cuda memory for out_vector");
        return cudaStatus;
    }

    cudaStatus = cudaMalloc((void**)&g_dev_data_int32.queue, sizeof(int) * sizeof(uint32_t) * g_dev_data_int32.queue_size);
    if (cudaStatus != cudaSuccess) {
        strcpy(g_error_message, "Failed to allocate cuda memory for input_vector");
        return cudaStatus;
    }


    cudaFree(g_dev_data_int32.out);
    cudaFree(g_dev_data_int32.queue);
    return cudaStatus;
}
