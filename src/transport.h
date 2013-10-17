#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

typedef struct {
    int (*reader) (int, char *, int, void *);
    int (*writer) (int, char *, int, void *);
    int (*poller) (int, double, int, void *);
    int (*peeker) (int, char *, int, void *);
    const char *(*errstr) (int, void *);
    void (*closer) (int, void *);
}transport_implementation;

typedef struct {
    transport_implementation *imp;
    void *ctx;
}transport_info;

/* Flags for select_fd's WAIT_FOR argument. */
enum {
  WAIT_FOR_READ = 1,
  WAIT_FOR_WRITE = 2
};

void fd_register_transport (int, transport_implementation *, void *);
void *fd_transport_context (int);

int fd_read (int, char *, int);
int fd_write (int, char *, int);
int fd_peek (int, char *, int);
int fd_poll (int, int, double);
const char *fd_errstr (int);
void fd_close (int);

int select_fd (int fd, double maxtime, int wait_for);

#endif // _TRANSPORT_H_
