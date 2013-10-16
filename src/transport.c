#include "transport.h"
#include "socket.h"

/* Register the transport layer operations that will be used when
   reading, writing, and polling FD.

   This should be used for transport layers like SSL that piggyback on
   sockets.  FD should otherwise be a real socket, on which you can
   call getpeername, etc.  */

void
fd_register_transport (int fd, struct transport_implementation *imp, void *ctx)
{
  struct transport_info *info;

  /* The file descriptor must be non-negative to be registered.
     Negative values are ignored by fd_close(), and -1 cannot be used as
     hash key.  */
  assert (fd >= 0);

  info = xnew (struct transport_info);
  info->imp = imp;
  info->ctx = ctx;
  if (!transport_map)
    transport_map = hash_table_new (0, NULL, NULL);
  hash_table_put (transport_map, (void *)(intptr_t) fd, info);
  ++transport_map_modified_tick;
}

/* Return context of the transport registered with
   fd_register_transport.  This assumes fd_register_transport was
   previously called on FD.  */

void *
fd_transport_context (int fd)
{
  struct transport_info *info = hash_table_get (transport_map, (void *)(intptr_t) fd);
  return info->ctx;
}

/* ... */
static bool
poll_internal (int fd, struct transport_info *info, int wf, double timeout)
{
  if (timeout == -1)
    timeout = opt.read_timeout;
  if (timeout)
    {
      int test;
      if (info && info->imp->poller)
        test = info->imp->poller (fd, timeout, wf, info->ctx);
      else
        test = sock_poll (fd, timeout, wf);
      if (test == 0)
        errno = ETIMEDOUT;
      if (test <= 0)
        return false;
    }
  return true;
}

/* Read no more than BUFSIZE bytes of data from FD, storing them to
   BUF.  If TIMEOUT is non-zero, the operation aborts if no data is
   received after that many seconds.  If TIMEOUT is -1, the value of
   opt.timeout is used for TIMEOUT.  */

int
fd_read (int fd, char *buf, int bufsize, double timeout)
{
  struct transport_info *info;
  LAZY_RETRIEVE_INFO (info);
  if (!poll_internal (fd, info, WAIT_FOR_READ, timeout))
    return -1;
  if (info && info->imp->reader)
    return info->imp->reader (fd, buf, bufsize, info->ctx);
  else
    return sock_read (fd, buf, bufsize);
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

int
fd_peek (int fd, char *buf, int bufsize, double timeout)
{
  struct transport_info *info;
  LAZY_RETRIEVE_INFO (info);
  if (!poll_internal (fd, info, WAIT_FOR_READ, timeout))
    return -1;
  if (info && info->imp->peeker)
    return info->imp->peeker (fd, buf, bufsize, info->ctx);
  else
    return sock_peek (fd, buf, bufsize);
}

/* Write the entire contents of BUF to FD.  If TIMEOUT is non-zero,
   the operation aborts if no data is received after that many
   seconds.  If TIMEOUT is -1, the value of opt.timeout is used for
   TIMEOUT.  */

int
fd_write (int fd, char *buf, int bufsize, double timeout)
{
  int res;
  struct transport_info *info;
  LAZY_RETRIEVE_INFO (info);

  /* `write' may write less than LEN bytes, thus the loop keeps trying
     it until all was written, or an error occurred.  */
  res = 0;
  while (bufsize > 0)
    {
      if (!poll_internal (fd, info, WAIT_FOR_WRITE, timeout))
        return -1;
      if (info && info->imp->writer)
        res = info->imp->writer (fd, buf, bufsize, info->ctx);
      else
        res = sock_write (fd, buf, bufsize);
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

const char *
fd_errstr (int fd)
{
  /* Don't bother with LAZY_RETRIEVE_INFO, as this will only be called
     in case of error, never in a tight loop.  */
  struct transport_info *info = NULL;
  if (transport_map)
    info = hash_table_get (transport_map, (void *)(intptr_t) fd);

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

void
fd_close (int fd)
{
  struct transport_info *info;
  if (fd < 0)
    return;

  /* Don't use LAZY_RETRIEVE_INFO because fd_close() is only called once
     per socket, so that particular optimization wouldn't work.  */
  info = NULL;
  if (transport_map)
    info = hash_table_get (transport_map, (void *)(intptr_t) fd);

  if (info && info->imp->closer)
    info->imp->closer (fd, info->ctx);
  else
    sock_close (fd);

  if (info)
    {
      hash_table_remove (transport_map, (void *)(intptr_t) fd);
      xfree (info);
      ++transport_map_modified_tick;
    }
}
