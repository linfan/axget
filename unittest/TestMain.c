#include "TestConn.h"
#include "axel.h"

#ifdef GEN_TRACE_FILE
FILE *axget_trace;
#endif

Suite * axget_suite(void)
{
    Suite *suite = suite_create ("TestAxget");

    TCase *tc_conn = tcase_create ("TextConn");
    tcase_add_test (tc_conn, test_conn_set);
    tcase_add_test (tc_conn, test_conn_url);
    suite_add_tcase (suite, tc_conn);

    return suite;
}

int main()
{
    int number_failed;
    Suite *s = axget_suite ();
    SRunner *sr = srunner_create (s);
    AXGET_MAIN_BEGIN

    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);

    AXGET_MAIN_LEAVE
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
