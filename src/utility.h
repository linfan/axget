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

/* trace */
#ifdef GEN_TRACE_FILE
extern FILE *axget_trace;
#define AXGET_FUN_BEGIN fprintf(axget_trace, "%s [ENTER] %s @ %s:%d\n", \
        __TIME__, __FUNCTION__, __FILE__, __LINE__); \
        fflush(axget_trace);
#define AXGET_FUN_LEAVE fprintf(axget_trace, "%s [LEAVE] %s @ %s:%d\n", \
        __TIME__, __FUNCTION__, __FILE__, __LINE__); \
        fflush(axget_trace);
#define AXGET_FUN_RETURN_D(RET) fprintf(axget_trace, "%s [LEAVE] %s @ %s:%d <Code %d>\n", \
        __TIME__, __FUNCTION__, __FILE__, __LINE__, RET); \
        fflush(axget_trace);
#define AXGET_FUN_RETURN_F(RET) fprintf(axget_trace, "%s [LEAVE] %s @ %s:%f <Value %d>\n", \
        __TIME__, __FUNCTION__, __FILE__, __LINE__, RET); \
        fflush(axget_trace);
#define AXGET_FUN_RETURN_S(RET) fprintf(axget_trace, "%s [LEAVE] %s @ %s:%s <Msg: %d>\n", \
        __TIME__, __FUNCTION__, __FILE__, __LINE__, RET); \
        fflush(axget_trace);
#define AXGET_MAIN_BEGIN \
    axget_trace = fopen("./axget.trc", "a"); \
    if (!axget_trace) \
    { \
        fprintf(stderr, "Open trace file failed !"); \
        AXGET_FUN_LEAVE; return RET_ERR; \
    } \
    fprintf(axget_trace, "-------- AXGET BEGIN --------\n"); \
    AXGET_FUN_BEGIN
#define AXGET_MAIN_LEAVE \
    AXGET_FUN_LEAVE \
    fprintf(axget_trace, "-------- AXGET END --------\n"); \
    fclose(axget_trace);
#else
#define AXGET_FUN_BEGIN
#define AXGET_FUN_LEAVE
#define AXGET_FUN_RETURN_D(RET)
#define AXGET_FUN_RETURN_F(RET)
#define AXGET_FUN_RETURN_S(RET)
#define AXGET_MAIN_BEGIN
#define AXGET_MAIN_LEAVE
#endif

double gettime();
int random_number (int max);

#endif // _UTILITY_H_
