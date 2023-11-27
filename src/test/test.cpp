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
#include <string.h>
#include <time.h>

#include <Windows.h>
#include <thread>


#include <stdio.h>
#include <string.h>
#include <time.h>

#include <Windows.h>
#include <thread>

#include <iostream>
#include <fstream>
//  ofstream - create and write
//  ifstream - read
//  fstream  - a combination

void func(int index)
{
    char message[256];
    strcpy(message, "This is test");
    int j = strlen(message);
    message[j] = index + '0';
    message[j+1] = '\0';
    struct timespec ts;

    for (int i = 0; i < 100000; i++) {
        int ret = timespec_get(&ts, TIME_UTC);
        libutils_write_log(message, (void*)&ts);
        //char buff[100];
        //strftime(buff, sizeof buff, "%D %T", gmtime(&ts.tv_sec));
        //printf("Current time: %s.%09ld UTC\n", buff, ts.tv_nsec);
        //std::cout << "count: " << i << std::endl;
    }
}


int main(int argc, char** argv)
{
    std::thread v[50];
    for (int i = 0; i < 50; i++) {
        v[i] = std::thread(func, i);
    }

    for (int i = 0; i < 50; i++) {
        v[i].join();
    }

    return 0;
}