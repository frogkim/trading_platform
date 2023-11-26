#ifdef LINUX

#else
#pragma once
#endif

#include <stdio.h>
#include <string.h>
#include <iostream>
#include "libfixprotocol.h"
//#include "libmath.h"
#include "libnetwork.h"
#include "libsql.h"
#include "libutils.h"

int main(int argc, char* argv[])
{
    std::cout << "Hello, world!" << std::endl;
    char test[30];
    strcpy_s(test, "This is test");
    writeLog(test);
    char* test2 = readLog();
    std::cout << test2 << std::endl;

    std::cout << "Press q key to end." << std::endl;
    char c = 0;
    while (c != 'q') {
        std::cin >> c;
    }
    
    return 0;
}