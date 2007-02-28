#ifndef COMPAT_SOCKET_H
#define COMPAT_SOCKET_H

#ifdef WIN32

#include <winsock2.h>

#include "compat_errno.h"

#ifndef NO_COMPAT_SOCKET

#ifdef select
#undef select
#endif
#ifdef close
#undef close
#endif
#ifdef write
#undef write
#endif
#ifdef read
#undef read
#endif

#define write socket_write
#define send socket_send
#define read socket_read
#define recv socket_recv
#define select socket_select
#define close socket_close
#define socket socket_socket
#define connect socket_connect
#define setsockopt socket_setsockopt

#endif

typedef int socklen_t;

extern int socket_close( int fd);
extern int socket_select( int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
extern int socket_socket(int domain, int type, int protocol);
extern int socket_connect( int  sockfd,  const  struct sockaddr *serv_addr, socklen_t addrlen);
extern int socket_send( int fd, const void *buf, size_t count, int flags);
extern int socket_write( int fd, const void *buf, size_t count);
extern int socket_recv( int fd, void *buf, size_t count, int flags);
extern int socket_read( int fd, void *buf, size_t count);
extern int socket_setsockopt( int fd, int  level,  int  optname,  const  void  *optval,socklen_t optlen);

#endif

#endif

