#ifdef LINUX
    #include <unistd.h>
#else
    #include <windows.h>
    #ifdef DEBUG
    // #pragma warning(suppress: 4996) // specific line
    #pragma warning(disable: 4996) // time structure
    #endif
#endif

#include "libfixprotocol.h"
//#include "libmath.h"
#include "libnetwork.h"
#include "libsql.h"
#include "libutils.h"

#include <stdio.h>
#include <iostream>
#include <fstream>
//  ofstream - create and write
//  ifstream - read
//  fstream  - a combination
#include <string.h>
#include <time.h>

#include <Windows.h>
#include <thread>


int main(int argc, char** argv)
{
    char message[256];
    
    struct timespec ts;
    for (int i = 0; i < 100; i++) {
        int ret = timespec_get(&ts, TIME_UTC);
        libutils_write_log("This is test", 0, (void*)&ts);
        Sleep(100);
        char buff[100];
        strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
        printf("Current time: %s.%09ld UTC\n", buff, ts.tv_nsec);
        //std::cout << "count: " << i << std::endl;
    }

    return 0;
}