#ifdef WIN32

#include <windows.h>
#include <winsock2.h>
#include <stdio.h>

#define NO_COMPAT_UNISTD
#undef NO_COMPAT_UNISTD

#define NO_COMPAT_ERRNO
#define NO_COMPAT_SELECT
#include "unistd.h"
#include "sys/select.h"
#undef NO_COMPAT_SELECT
#include "compat_errno.h"
#undef NO_COMPAT_ERRNO

#define NO_COMPAT_SOCKET
#include "sys/socket.h"
#undef NO_COMPAT_SOCKET

#if defined(strerror)||defined(socket)||defined(connect)||defined(send)||defined(strerror)
#error "Somebody f*cked up headers"
#endif

int h_errno;

int unistd_select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout)
{
	DWORD dw, to;
	HANDLE h;
	int ret, flag, i, n;

	errno = 0;
//	printf( "%s..\n", __func__);fflush( stdout);
	to = timeout ? timeout->tv_sec * 1000 + timeout->tv_usec / 1000 : INFINITE;
	ret = 0;
	n = readfds->fd_count - 1;
//	printf( "about to loop n=%d\n", n);fflush( stdout);
	for (i = n; i >= 0; i--)
	{
//		printf( "about to fake read h=%d to=%lu\n", (int)h, to);fflush( stdout);
		h = (HANDLE)readfds->fd_array[0];
		dw = PIPE_NOWAIT;
		SetNamedPipeHandleState( h, &dw, NULL, NULL);
		flag = ReadFile(h,&dw,0,&dw,NULL);
		dw = 0;
		SetNamedPipeHandleState( h, &dw, NULL, NULL);
//		printf( "fake read returned %d dwread=%08lx\n", ret, dw);fflush(stdout);
		if (flag)
			ret++;
		else
			FD_CLR( (int)h, readfds);
	}

	return ret;
}

int unistd_read( int fd, void *buf, size_t count)
{
	int result = 0;
	DWORD ret;

//	printf( "%s\n", __func__);
//	result = read( fd, buf, count);
	ReadFile((HANDLE)fd,(LPVOID)buf, (DWORD)count, &ret, NULL);
	result = (int)ret;

	return result;
}

int unistd_write( int fd, const void *buf, size_t count)
{
	int result = 0;
	DWORD ret;

//	printf( "%s\n", __func__);
	WriteFile((HANDLE)fd,(LPCVOID)buf, (DWORD)count, &ret, NULL);
	result = (int)ret;

	return result;
}

int unistd_pipe( int filedes[2])
{
	int result = 0;

//	printf( "%s\n", __func__);
	errno = 0;
	if (filedes)
	{
		if (!CreatePipe((PHANDLE)&filedes[0], (PHANDLE)&filedes[1], NULL, 0))
		{
			errno = EFAULT;
			result = -1;
		}
	}
	else
	{
		errno = EFAULT;
		result = -1;
	}

	return result;
}

int unistd_close( int fd)
{
	int result = 0;

//	printf( "%s : fd=%d\n", __func__, fd);
	errno = 0;
	if (!CloseHandle( (HANDLE)fd))
	{
		errno = EBADF;
		result = -1;
	}
	else
	{
		result = 1;
	}

	return result;
}

void unistd_FD_SET( int fd, fd_set *set)
{
	if (fd)
		FD_SET( fd, set);
}

int unistd_WSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData)
{
	int result = 0;

	return result;
}

int compat_socket_init()
{
	int result = 0;
	static int is_init = 0;

//	printf( "%s: is_init=%d\n", __func__, is_init);
	if (!is_init)
	{
		is_init = 1;
	
		WSADATA wsaData;
	    int iResult;
	
	    iResult = WSAStartup( MAKEWORD( 2, 2), &wsaData);
	    if (iResult)
	    {
	         printf( "WSAStartup failed: %d\n", iResult);
		     result = -1;
	    }
	}

	return result;
}

char * string_strerror( compat_errno errnum)
{
	char *result;

//	printf( "%s : errnum=%d\n", __func__, errnum);fflush( stdout);
	switch (errnum)
	{
		case ECONNREFUSED:result = MCONNREFUSED;break;
		case EAFNOSUPPORT:result = MAFNOSUPPORT;break;
		case ENOTSOCK:result = MNOTSOCK;break;
		case ENOPROTOOPT:result = MNOPROTOOPT;break;
		case EPROTONOSUPPORT:result = MPROTONOSUPPORT;break;
		case ESOCKTNOSUPPORT:result = MSOCKTNOSUPPORT;break;
		case EADDRINUSE:result = MADDRINUSE;break;
		case ENETUNREACH:result = MNETUNREACH;break;
		case EISCONN:result = MISCONN;break;
		case ETIMEDOUT:result = MTIMEDOUT;break;
		case EALREADY:result = MALREADY;break;

#if 0
		case ENONE:
#else
		default:
#endif
			printf( "[%s : handing %d to _strerror] ", __func__, errnum);fflush( stdout);
			result = strerror( errnum);
			break;
	}

	return result;
}

void stdio_perror( const char *s)
{
	printf( "%s\n", __func__);fflush( stdout);
	printf( "%s: %s\n", s, string_strerror( errno));
}

int socket_errno()
{
	int _errno, err;

//	printf( "socket_errno\n");fflush( stdout);
	err = WSAGetLastError();
	printf( "%s: err=%d\n", __func__, err);
	switch (err)
		{
			case WSAEACCES:
				_errno = EACCES;
				break;
			case WSAEADDRINUSE:
				_errno = EADDRINUSE;
				break;
			case WSAEINTR:
				_errno = EINTR;
				break;
			case WSAEALREADY:
				_errno = EALREADY;
				break;
			case WSAECONNREFUSED:
				_errno = ECONNREFUSED;
				break;
			case WSAEFAULT:
				_errno = EFAULT;
				break;
			case WSAEISCONN:
				_errno = EISCONN;
				break;
			case WSAENETUNREACH:
				_errno = ENETUNREACH;
				break;
			case WSAESOCKTNOSUPPORT:
				_errno = EINVAL;
				break;
			case WSAEPROTONOSUPPORT:
				_errno = EPROTONOSUPPORT;
				break;
			case WSAEAFNOSUPPORT:
				_errno = EAFNOSUPPORT;
				break;
			case ERROR_INVALID_HANDLE:
				_errno = EBADF;
				break;
			case WSAEINVAL:
				_errno = EINVAL;
				break;

			case WSANOTINITIALISED:
//				printf( "WSA not initialized !!\n");
				compat_socket_init();
				_errno = EAGAIN;
				break;

			case WSAENOTSOCK:
//				printf( "NOT SOCK!!!\n");fflush( stdout);
				_errno = ENOTSOCK;
				break;
			
			case WSAENETDOWN:
			case WSAEINPROGRESS:
			case WSAEADDRNOTAVAIL:
			case WSAEHOSTUNREACH:
			case WSAENOBUFS:
			case WSAETIMEDOUT:
			case WSAEWOULDBLOCK:
			default:
				printf( "%s: unknown error %d\n", __func__, err);fflush( stdout);
//				_errno = -1;
				_errno = 0;
				break;
		}
//	printf( "%s: err=%d returning %d\n", __func__, err, _errno);
	return _errno;
}

int socket_select( int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout)
{
	int result = 0;

//	printf( "%s\n", __func__);
	errno = 0;
	result = select( nfds, readfds, writefds, exceptfds, timeout);
	if (result == SOCKET_ERROR)
	{
//		printf( "%s : error\n", __func__);fflush( stdout);
		errno = socket_errno();
		if (errno == ENOTSOCK)
		{
//			printf( "%s : calling unistd_select\n", __func__);fflush( stdout);
			result = unistd_select( nfds, readfds, writefds, exceptfds, timeout);
		}
		else
		{
			result = -1;
		}
	}
	else
	{
//		printf( "%s : no error\n", __func__);fflush( stdout);
	}

	return result;
}

int socket_connect( int  sockfd,  const  struct sockaddr *serv_addr, socklen_t addrlen)
{
	int result = 0;

//	printf( "%s\n", __func__);
	errno = 0;
	if (connect( (SOCKET)sockfd, serv_addr, (int)addrlen) == SOCKET_ERROR)
	{
		errno = socket_errno();
		result = -1;
	}

	return result;
}

int socket_close( int fd)
{
	int result = 0;

//	printf( "%s\n", __func__);
	errno = 0;
	if (shutdown( (SOCKET)fd, SD_SEND) != SOCKET_ERROR)
	{
		if (closesocket( (SOCKET)fd) == SOCKET_ERROR)
		{
			errno = socket_errno();
			result = -1;
		}
	}
	else
	{
		errno = socket_errno();
		if (errno == ENOTSOCK)
		{
//			printf( "%s : calling unistd_close\n", __func__);fflush( stdout);
			result = unistd_close( fd);
		}
		else
		{
			result = -1;
		}
	}

	return result;
}

int socket_socket( int domain, int type, int protocol)
{
	int result;

//	printf( "%s\n", __func__);fflush(stdout);
	while (1)
	{
		errno = 0;
//		printf( "compat_socket : about to call socket\n");fflush(stdout);
		if ((result = socket( domain, type, protocol)) == INVALID_SOCKET)
		{
//			printf( "compat_socket: invalid socket\n");fflush( stdout);
			errno = socket_errno();
			if (errno == EAGAIN)
				continue;
			result = -1;
		}
		else
		{
//			printf( "compat_socket : not invalid, sock=%d\n", result);fflush( stdout);
		}
		break;
	}
//	printf( "compat_socket finished\n");fflush( stdout);

	return result;
}

int socket_send( int fd, const void *buf, size_t count, int flags)
{
	int result = 0;

//	printf( "%s\n", __func__);
	errno = 0;
	if (send( fd, buf, count, flags) == SOCKET_ERROR)
	{
		errno = socket_errno();
		if (errno == ENOTSOCK)
		{
			result = unistd_write( fd, buf, count);
		}
		else
			result = -1;
	}

	return result;
}

int socket_write( int fd, const void *buf, size_t count)
{
//	printf( "%s\n", __func__);

	return socket_send( fd, buf, count, 0);
}

int socket_setsockopt( int fd, int  level,  int  optname,  const  void  *optval,socklen_t optlen)
{
	int result = 0;

//	printf( "%s\n", __func__);
	errno = 0;
	if (setsockopt( fd, level, optname, optval, optlen) == SOCKET_ERROR)
	{
		errno = socket_errno();
		result = -1;
	}

	return result;
}


#endif

