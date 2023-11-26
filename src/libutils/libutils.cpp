#include "libutils_header.h"

static char* g_s;

void writeLog(char* s)
{
    g_s = s;    
}

char* readLog(void)
{
    return g_s;
}