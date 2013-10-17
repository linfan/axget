#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include "transport.h"
#include "socket.h"
#include "utility.h"

/* Register the transport layer operations that will be used when
   reading, writing, and polling FD.

   This should be used for transport layers like SSL that piggyback on
   sockets.  FD should otherwise be a real socket, on which you can
   call getpeername, etc.  */

static transport_info *info = NULL;
void fd_register_transport (int fd, transport_implementation *imp, void *ctx)
{
    /* The file descriptor must be non-negative to be registered.
       Negative values are ignored by fd_close(), and -1 cannot be used as
       hash key.  */
    assert (fd >= 0);

    if (info != NULL)
        free(info);
    info = malloc(sizeof(transport_info));
    info->imp = imp;
    info->ctx = ctx;
}

/* Return context of the transport registered with
   fd_register_transport.  This assumes fd_register_transport was
   previously called on FD.  */

void* fd_transport_context (int fd)
{
    return info->ctx;
}

/* Wait for socket to be ready */
int fd_poll (int fd, int wf, double timeout)
{
    if (timeout)
    {
        int test = info->imp->poller (fd, timeout, wf, info->ctx);
        if (test == 0)
            errno = ETIMEDOUT;
        if (test <= 0)
            return RET_FAIL;
    }
    return RET_OK;
}

/* Read no more than BUFSIZE bytes of data from FD, storing them to
   BUF.  If TIMEOUT is non-zero, the operation aborts if no data is
   received after that many seconds.  If TIMEOUT is -1, the value of
   opt.timeout is used for TIMEOUT.  */

int fd_read (int fd, char *buf, int bufsize)
{
    return info->imp->reader (fd, buf, bufsize, info->ctx);
}

/* Like fd_read, except it provides a "preview" of the data that will
   be read by subsequent calls to fd_read.  Specifically, it copies no
   more than BUFSIZE bytes of the currently available data to BUF and
   returns the number of bytes copied.  Return values and timeout
   semantics are the same as those of fd_read.

CAVEAT: Do not assume that the first subsequent call to fd_read
will retrieve the same amount of data.  Reading can return more or
less data, depending on the TCP implementation and other
circumstances.  However, barring an error, it can be expected that
all the peeked data will eventually be read by fd_read.  */

int fd_peek (int fd, char *buf, int bufsize)
{
    return info->imp->peeker (fd, buf, bufsize, info->ctx);
}

/* Write the entire contents of BUF to FD.  If TIMEOUT is non-zero,
   the operation aborts if no data is received after that many
   seconds.  If TIMEOUT is -1, the value of opt.timeout is used for
   TIMEOUT.  */

int fd_write (int fd, char *buf, int bufsize)
{
    int res;

    /* `write' may write less than LEN bytes, thus the loop keeps trying
       it until all was written, or an error occurred.  */
    res = 0;
    while (bufsize > 0)
    {
        res = info->imp->writer (fd, buf, bufsize, info->ctx);
        if (res <= 0)
            break;
        buf += res;
        bufsize -= res;
    }
    return res;
}

/* Report the most recent error(s) on FD.  This should only be called
   after fd_* functions, such as fd_read and fd_write, and only if
   they return a negative result.  For errors coming from other calls
   such as setsockopt or fopen, strerror should continue to be
   used.

   If the transport doesn't support error messages or doesn't supply
   one, strerror(errno) is returned.  The returned error message
   should not be used after fd_close has been called.  */

const char * fd_errstr (int fd)
{
    if (info && info->imp->errstr)
    {
        const char *err = info->imp->errstr (fd, info->ctx);
        if (err)
            return err;
        /* else, fall through and print the system error. */
    }
    return strerror (errno);
}

/* Close the file descriptor FD.  */

void fd_close (int fd)
{
    info->imp->closer (fd, info->ctx);

    if (info)
    { free(info); }
}


/* Wait for a single descriptor to become available, timing out after
 * MAXTIME seconds.  Returns 1 if FD is available, 0 for timeout and
 * -1 for error.  The argument WAIT_FOR can be a combination of
 * WAIT_FOR_READ and WAIT_FOR_WRITE.
 *
 * This is a mere convenience wrapper around the select call, and
 * should be taken as such (for example, it doesn't implement Wget's
 * 0-timeout-means-no-timeout semantics.)  */

int select_fd (int fd, double maxtime, int wait_for)
{
    fd_set fdset;
    fd_set *rd = NULL, *wr = NULL;
    struct timeval tmout;
    int result;

    FD_ZERO (&fdset);
    FD_SET (fd, &fdset);
    if (wait_for & WAIT_FOR_READ)
        rd = &fdset;
    if (wait_for & WAIT_FOR_WRITE)
        wr = &fdset;

    tmout.tv_sec = (long) maxtime;
    tmout.tv_usec = 1000000 * (maxtime - (long) maxtime);

    do
    {
        result = select (fd + 1, rd, wr, NULL, &tmout);
    }
    while (result < 0 && errno == EINTR);

    return result;
}
