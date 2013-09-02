#include <stdio.h>
#include <string.h>
#include "common.h"
#include "axel.h"

START_TEST (test_conn_set)
{
    conn_t *conn = malloc(sizeof(conn_t));
    char* url = malloc(MAX_STRING);

    strcpy(url, "www.nsn.com");
    conn_set(conn, url);
    ck_assert_str_eq(conn->host, "www.nsn.com");
    ck_assert_int_eq(conn->port, 80);
    ck_assert_str_eq(conn->dir, "/");
    ck_assert_str_eq(conn->file, "");
    ck_assert_str_eq(conn->user, "");
    ck_assert_str_eq(conn->pass, "");

    //strcpy(url, "www.nsn.com/folder/news/index.html");
    //conn_set(conn, url);
    //fprintf(stderr, "'%s' -> \nhost: %s \nport: %d \ndir: %s \nfile: %s \nuser: %s \npwd: %s \n",
    //        url, conn->host, conn->port, conn->dir, conn->file, conn->user, conn->pass);

    //strcpy(url, "http://falin:abcacb@www.nsn.com:99/folder/older/index.html");
    //conn_set(conn, url);
    //fprintf(stderr, "'%s' -> \nhost: %s \nport: %d \ndir: %s \nfile: %s \nuser: %s \npwd: %s \n",
    //        url, conn->host, conn->port, conn->dir, conn->file, conn->user, conn->pass);

    free(conn);
    free(url);
}
END_TEST

