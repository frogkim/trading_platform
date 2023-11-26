#ifndef LIBMATH_H_
#define LIBMATH_H_

#ifdef LINUX
#ifdef EXPORT
#define LIBMATH __attribute__((visibility("default")))
#else
#define LIBMATH
#endif

#else
#ifdef EXPORT
#define LIBMATH __declspec(dllexport)
#else
#define LIBMATH __declspec(dllimport)
#endif

#endif  // CUDALIB_EXPORT

// Math library
// Support Target: cuda, x64

extern "C" 
{
    LIBMATH bool isError(void);
    LIBMATH char* getLastErrsorMessage_pChar(void);
    LIBMATH bool DeviceReset(void);
    LIBMATH bool addWithCuda(int* c, const int* a, const int* b, unsigned int size);
    LIBMATH bool addWithCudaBegin(int* out, const int* input_a, const int* input_b, unsigned int size);
    LIBMATH bool addWithCudaContinue(const int* input); // c = c + b
    LIBMATH bool addWithCudaTerminate(void);
}
#endif

