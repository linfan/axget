#ifndef _UTILITY_H_
#define _UTILITY_H_

typedef int bool;
#define true    1
#define false   0

#define RET_OK  1
#define RET_ERR 0
#define RET_FAIL RET_ERR

#define min( a, b )     ( (a) < (b) ? (a) : (b) )
#define max( a, b )     ( (a) > (b) ? (a) : (b) )

/* type of echo */
enum log_options 
{ 
    ERROR_LOG = 0,
    WORK_LOG  = 1,
    INFO_LOG  = 2,
    DEBUG_LOG = 3
};

/* log */
#define PTR_FORMAT(p) (int) (2 * sizeof (void *)), (unsigned long) (p)
void echo(const int type, const char* format, ...);
extern int verbose;

/* debug */
#ifdef DEBUG
# define IF_DEBUG if (1)
#else
# define IF_DEBUG if (0)
#endif

double gettime();
int random_number (int max);

#endif // _UTILITY_H_
