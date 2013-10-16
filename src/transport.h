#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

struct transport_implementation {
    int (*reader) (int, char *, int, void *);
    int (*writer) (int, char *, int, void *);
    int (*poller) (int, double, int, void *);
    int (*peeker) (int, char *, int, void *);
    const char *(*errstr) (int, void *);
    void (*closer) (int, void *);
};

void fd_register_transport (int, struct transport_implementation *, void *);
void *fd_transport_context (int);

int fd_read (int, char *, int, double);
int fd_write (int, char *, int, double);
int fd_peek (int, char *, int, double);
const char *fd_errstr (int);
void fd_close (int);

#endif // _TRANSPORT_H_
