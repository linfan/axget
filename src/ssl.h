#ifndef _SSH_H_
#define _SSH_H_

bool ssl_init (conf_t *opt);
int ssl_connect(int fd, const char *hostname);
bool ssl_check_certificate (int, const char *, conf_t *opt);

#endif // _SSH_H_
