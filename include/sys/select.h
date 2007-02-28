#ifndef COMPAT_SELECT_H
#define COMPAT_SELECT_H

#ifndef NO_COMPAT_SELECT

#ifndef select

#define select unistd_select

#endif

#undef FD_SET
#define FD_SET unistd_FD_SET

#endif

extern int unistd_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
extern void unistd_FD_SET( int fd, fd_set *set);
extern int unistd_WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData);

#endif

