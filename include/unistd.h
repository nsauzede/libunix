#ifndef COMPAT_UNISTD_H
#define COMPAT_UNISTD_H

#ifdef WIN32

#include <windows.h>

#include "compat_errno.h"
#include "sys/select.h"

#define usleep(t) Sleep(t / 1000)
#define sleep(t) _sleep(t * 1000)

#ifndef NO_COMPAT_UNISTD

#ifndef read
#define read unistd_read
#endif
#ifndef write
#define write unistd_write
#endif
#define pipe unistd_pipe
#ifndef close
#define close unistd_close
#endif

#else

extern int read( int fd, void *buf, size_t count);
extern int write( int fd, const void *buf, size_t count);

#endif

extern int unistd_read( int fd, void *buf, size_t count);
extern int unistd_write( int fd, const void *buf, size_t count);

extern int unistd_pipe( int filedes[2]);
extern int unistd_close( int fd);

#endif

#endif

