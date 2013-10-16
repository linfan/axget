#ifndef _UTILITY_H_
#define _UTILITY_H_

/* type of echo */
#define ERROR_LOG 0
#define WORK_LOG  1
#define INFO_LOG  2
#define DEBUG_LOG 3

double gettime();
void echo(const int type, const char* format, ...);

extern int verbose;

#endif // _UTILITY_H_
