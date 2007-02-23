#ifndef COMPAT_UNISTD_H
#define COMPAT_UNISTD_H

#ifdef WIN32

#include <windows.h>

#include "sys/select.h"

#define usleep(t) Sleep(t / 1000)

#ifndef NO_COMPAT_UNISTD

#define read unistd_read
#define write unistd_write
#define pipe unistd_pipe

#else

extern int read( int fd, void *buf, size_t count);
extern int write( int fd, void *buf, size_t count);

#endif

extern int unistd_read( int fd, void *buf, size_t count);
extern int unistd_write( int fd, void *buf, size_t count);

extern int unistd_pipe( int filedes[2]);

#endif

#endif

