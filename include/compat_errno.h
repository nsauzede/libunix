#ifndef COMPAT_ERRNO_H
#define COMPAT_ERRNO_H

#ifdef WIN32

#include <errno.h>

#define ENOTSOCK		-666	/* fd not a socket */

#define EPROTONOSUPPORT 93      /* Protocol not supported */
#define EAFNOSUPPORT    97  	/* Address family not supported by protocol */
#define EADDRINUSE      98      /* Address already in use */
#define ENETUNREACH     101     /* Network is unreachable */
#define EISCONN         106     /* Transport endpoint is already connected */
#define ECONNREFUSED    111     /* Connection refused */
#define EALREADY        114     /* Operation already in progress */

#endif

#endif

