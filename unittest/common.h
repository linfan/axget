#ifndef __COMMON_H__
#define __COMMON_H__

#include <check.h>

#undef START_TEST
#define START_TEST(__testname) \
    void __testname (int _i CK_ATTRIBUTE_UNUSED) \
    { \
        tcase_fn_start (""# __testname, __FILE__, __LINE__);

#define DECLEAR_TEST(__testname) \
    void __testname (int _i);

#endif
