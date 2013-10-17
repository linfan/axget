#include <errno.h>
//#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "transport.h"
#include "utility.h"

/* Basic socket operations, mostly EINTR wrappers.  */

static int sock_read (int fd, char *buf, int bufsize, void *arg)
{
    int res;
    do
        res = read (fd, buf, bufsize);
    while (res == -1 && errno == EINTR);
    return res;
}

static int sock_write (int fd, char *buf, int bufsize, void *arg)
{
    int res;
    do
        res = write (fd, buf, bufsize);
    while (res == -1 && errno == EINTR);
    return res;
}

static int sock_poll (int fd, double timeout, int wait_for, void *arg)
{
    return select_fd (fd, timeout, wait_for);
}

static int sock_peek (int fd, char *buf, int bufsize, void *arg)
{
    int res;
    do
        res = recv (fd, buf, bufsize, MSG_PEEK);
    while (res == -1 && errno == EINTR);
    return res;
}

static const char* sock_errstr (int fd, void *arg)
{
    return (const char*)strerror((int)errno);
}

static void sock_close (int fd, void *arg)
{
    close (fd);
    echo(WORK_LOG, "Closed fd %d\n", fd);
}

/* openssl_transport is the singleton that describes the SSL transport
   methods provided by this file.  */
static transport_implementation sock_transport =
{
    sock_read, sock_write, sock_poll,
    sock_peek, sock_errstr, sock_close
};

int socket_connect(int fd)
{
    fd_register_transport(fd, &sock_transport, NULL);
    return RET_OK;
}

