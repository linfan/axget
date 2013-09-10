#include <stdio.h>
#include <string.h>
#include "common.h"
#include "axel.h"

START_TEST (test_conn_set)
{
    conn_t *conn = malloc(sizeof(conn_t));
    char* url = malloc(MAX_STRING);

    strcpy(url, "www.google.com");
    conn_set(conn, url);
    ck_assert_str_eq(conn->host, "www.google.com");
    ck_assert_int_eq(conn->port, 80);
    ck_assert_str_eq(conn->dir, "/");
    ck_assert_str_eq(conn->file, "");
    ck_assert_str_eq(conn->user, "");
    ck_assert_str_eq(conn->pass, "");
    ck_assert_str_eq(conn->args, "");

    strcpy(url, "www.yahoo.com/folder/news/index.html");
    conn_set(conn, url);
    ck_assert_str_eq(conn->host, "www.yahoo.com");
    ck_assert_int_eq(conn->port, 80);
    ck_assert_str_eq(conn->dir, "/folder/news/");
    ck_assert_str_eq(conn->file, "index.html");
    ck_assert_str_eq(conn->user, "");
    ck_assert_str_eq(conn->pass, "");
    ck_assert_str_eq(conn->args, "");

    strcpy(url, "http://u5er:passw0rd@www.yahoo.com:8090/folder/older/index.html?id=9734&ad=6158");
    conn_set(conn, url);
    ck_assert_str_eq(conn->host, "www.yahoo.com");
    ck_assert_int_eq(conn->port, 8090);
    ck_assert_str_eq(conn->dir, "/folder/older/");
    ck_assert_str_eq(conn->file, "index.html");
    ck_assert_str_eq(conn->user, "u5er");
    ck_assert_str_eq(conn->pass, "passw0rd");
    ck_assert_str_eq(conn->args, "?id=9734&ad=6158");

    strcpy(url, "ftp://www.yahoo.com/folder/older/index.html");
    conn_set(conn, url);
    ck_assert_str_eq(conn->host, "www.yahoo.com");
    ck_assert_int_eq(conn->port, 21);
    ck_assert_str_eq(conn->dir, "/folder/older/");
    ck_assert_str_eq(conn->file, "index.html");
    ck_assert_str_eq(conn->user, "anonymous");
    ck_assert_str_eq(conn->pass, "mailto:axel-devel@lists.alioth.debian.org");
    ck_assert_str_eq(conn->args, "");

    free(conn);
    free(url);
}
END_TEST

START_TEST (test_conn_url)
{
    conn_t *conn = malloc(sizeof(conn_t));
    char *url;

    strcpy(conn->host, "www.yahoo.com");
    conn->port = 8090;
    strcpy(conn->dir , "/folder/older/");
    strcpy(conn->file, "index.html");
    strcpy(conn->user, "u5er");
    strcpy(conn->pass, "passw0rd");
    strcpy(conn->args, "?id=9734&ad=6158");
    url = conn_url(conn);
    ck_assert_str_eq(url, "http://u5er:passw0rd@www.yahoo.com:8090/folder/older/index.html?id=9734&ad=6158");

    free(conn);
}
END_TEST

