#include "check.h"
#include "axel.h"

#ifdef GEN_TRACE_FILE
FILE *axget_trace;
#endif

void Test_conn_set();

int main()
{
    AXGET_MAIN_BEGIN

    Test_conn_set();

    AXGET_MAIN_LEAVE
    return 0;
}
