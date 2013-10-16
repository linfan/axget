#include <time.h>
#include <sys/time.h>
#include <stdarg.h>
#include <stdio.h>
#include "utility.h"

/* global verbose level */
int verbose = 0;

// Print work information
void echo(int type, const char* format, ...)
{
    va_list args;
    if (type >= verbose)
    {
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

/* time() with more precision                       */
double gettime()
{
    struct timeval time[1];
    gettimeofday(time, 0);
    return((double) time->tv_sec + (double) time->tv_usec / 1000000);
}

