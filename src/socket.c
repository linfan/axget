
/* openssl_transport is the singleton that describes the SSL transport
   methods provided by this file.  */

static struct transport_implementation sock_transport =
{
    sock_read, sock_write, sock_poll,
    sock_peek, sock_errstr, sock_close
};

/* Basic socket operations, mostly EINTR wrappers.  */

static int sock_read (int fd, char *buf, int bufsize)
{
    int res;
    do
        res = read (fd, buf, bufsize);
    while (res == -1 && errno == EINTR);
    return res;
}

static int sock_write (int fd, char *buf, int bufsize)
{
    int res;
    do
        res = write (fd, buf, bufsize);
    while (res == -1 && errno == EINTR);
    return res;
}

static int sock_poll (int fd, double timeout, int wait_for)
{
    return select_fd (fd, timeout, wait_for);
}

static int sock_peek (int fd, char *buf, int bufsize)
{
    int res;
    do
        res = recv (fd, buf, bufsize, MSG_PEEK);
    while (res == -1 && errno == EINTR);
    return res;
}

static const char * sock_errstrvoid (int fd, void *arg)
{
    return strerror (errno);
}

static void sock_close (int fd)
{
    close (fd);
    (("Closed fd %d\n", fd));
}

void socket_init(int fd, const char *hostname)
{
    fd_register_transport(fd, &sock_transport, ctx);
}

