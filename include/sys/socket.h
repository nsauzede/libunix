#ifndef COMPAT_SOCKET_H
#define COMPAT_SOCKET_H

#ifdef WIN32

#include <winsock2.h>

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
#define write socket_write
#define select socket_select
#define close socket_close
#define socket socket_socket
#define connect socket_connect
#define strerror string_strerror
#define perror stdio_perror

#endif

typedef int socklen_t;

extern int socket_close( int fd);
extern int socket_select( int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);
extern int socket_socket(int domain, int type, int protocol);
extern int socket_connect( int  sockfd,  const  struct sockaddr *serv_addr, socklen_t addrlen);
extern char * string_strerror( int errnum);
extern void stdio_perror( const char *s);
extern int socket_write( int fd, const void *buf, size_t count);

#endif

#endif

