#ifndef COMPAT_SOCKET_H
#define COMPAT_SOCKET_H

#ifdef WIN32

#include <winsock2.h>

#ifndef NO_COMPAT_SOCKET

#ifdef select
#undef select
#endif
#define select socket_select
#define close socket_close
#define socket socket_socket

#endif

typedef int socklen_t;

extern int socket_close( int fd);
extern int socket_select( int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
extern int socket_socket(int domain, int type, int protocol);

#endif

#endif

