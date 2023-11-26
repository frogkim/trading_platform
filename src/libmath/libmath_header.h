#ifndef CUDALIB_HEADER_H_
#define CUDALIB_HEADER_H_
#include "libmath.h"
#include "types_and_global.h"

bool SetDevice(int index);

__global__ void _addKernel(int* out, const int* input_a, const int* input_b);
cudaError_t _addWithCuda(int* out, const int* input_a, const int* input_b, unsigned int size);
cudaError_t _addWithCudaQueue(void);
#endif