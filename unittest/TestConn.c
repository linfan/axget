#include <stdio.h>
#include <string.h>
#include "axel.h"

void Test_conn_set()
{
    conn_t *conn = malloc(sizeof(conn_t));
    char* url = malloc(MAX_STRING);

    fprintf(stderr, "--------------");
    strcpy(url, "www.nsn.com");
    conn_set(conn, url);
    fprintf(stderr, "'%s' -> \nhost: %s \nport: %d \ndir: %s \nfile: %s \nuser: %s \npwd: %s \n",
            url, conn->host, conn->port, conn->dir, conn->file, conn->user, conn->pass);

    fprintf(stderr, "--------------");
    strcpy(url, "www.nsn.com/folder/news/index.html");
    conn_set(conn, url);
    fprintf(stderr, "'%s' -> \nhost: %s \nport: %d \ndir: %s \nfile: %s \nuser: %s \npwd: %s \n",
            url, conn->host, conn->port, conn->dir, conn->file, conn->user, conn->pass);

    fprintf(stderr, "--------------");
    strcpy(url, "http://falin:abcacb@www.nsn.com:99/folder/older/index.html");
    conn_set(conn, url);
    fprintf(stderr, "'%s' -> \nhost: %s \nport: %d \ndir: %s \nfile: %s \nuser: %s \npwd: %s \n",
            url, conn->host, conn->port, conn->dir, conn->file, conn->user, conn->pass);

    free(conn);
    free(url);
}

