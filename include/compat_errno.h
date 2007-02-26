#ifndef COMPAT_ERRNO_H
#define COMPAT_ERRNO_H

#ifdef WIN32

#include <errno.h>

#if 0
#define ENOTSOCK		88		/* fd not a socket */
#define ENOPROTOOPT 	92      /* Protocol not supported */
#define EPROTONOSUPPORT 93      /* Protocol not supported */
#define ESOCKTNOSUPPORT 94      /* Protocol not supported */
#define EAFNOSUPPORT    97  	/* Address family not supported by protocol */
#define EADDRINUSE      98      /* Address already in use */
#define ENETUNREACH     101     /* Network is unreachable */
#define EISCONN         106     /* Transport endpoint is already connected */
#define ECONNREFUSED    111     /* Connection refused */
#define EALREADY        114     /* Operation already in progress */
#else
typedef enum {
	ENONE=		-1,
ENOTSOCK=		88,		/* Socket operation on non-socket */
 ENOPROTOOPT =92    , /* Protocol not available */
 EPROTONOSUPPORT=93 ,    /* Protocol not supported */
 ESOCKTNOSUPPORT=94 ,    /* Socket type not supported */
 EAFNOSUPPORT  = 97 ,	/* Address family not supported by protocol */
 EADDRINUSE    = 98 ,    /* Address already in use */
 ENETUNREACH   = 101,    /* Network is unreachable */
 EISCONN       = 106,    /* Transport endpoint is already connected */
ECONNREFUSED   =111,    /* Connection refused */
EALREADY  =     114 ,   /* Operation already in progress */
} compat_errno;
#endif

#define MNOTSOCK		"Socket operation on non-socket"
#define MNOPROTOOPT		"Protocol not available"
#define MPROTONOSUPPORT	"Protocol not supported"
#define MSOCKTNOSUPPORT	"Socket type not supported"
#define MAFNOSUPPORT	"Address family not supported by protocol"
#define MADDRINUSE		"Address already in use"
#define MNETUNREACH		"Network is unreachable"
#define MISCONN			"Transport endpoint is already connected"
#define MCONNREFUSED	"Connection refused"
#define MALREADY		"Operation already in progress"

#ifndef NO_COMPAT_ERRNO
#define perror stdio_perror
#define strerror string_strerror
#endif

//extern char * string_strerror( int errnum);
extern char * string_strerror( compat_errno errnum);
extern void stdio_perror( const char *s);

#endif

#endif

