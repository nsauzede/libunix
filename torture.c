#include <stdio.h>

int main_no_net();
int main_net();

int main()
{
	int result;

	printf( "### Testing Non-net functions.. ###\n");
	result = main_no_net();
	if (result)
		printf( "--- Error at %s:%d ---\n", __FILE__, result);
	else
		printf( "+++ Feeling groovy +++\n");

	printf( "### Testing Net functions.. ###\n");
	result = main_net();
	if (result)
		printf( "--- Error at %s:%d ---\n", __FILE__, result);
	else
		printf( "+++ Feeling groovy ++\n");

	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main_no_net()
{
	int p = -1;
	int fds[2];
	int result = 0;
	int _err, ret, _ret;
	char *func;

#ifndef SOL8
	_err = EFAULT;
	_ret = -1;
	func = "pipe";		// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = p = pipe( NULL);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
#endif
	_err = EBADF;
	_ret = -1;
	func = "close";			// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = close( -666);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	_err = 0;
	_ret = 0;
	func = "pipe";		// should work
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = p = pipe( fds);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
#if 0
	_err = 0;
	_ret = 0;
	func = "close";		// should work
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = close( p);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
#endif

	result = 0;
err:

	return result;
}

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

int main_net()
{
	int result = 0;
	int sock;
	struct sockaddr_in sa;
	char buf[100];
	int n;
	socklen_t len;
	int _err, ret, _ret;
	char *func;
	int on;

	_err = EAFNOSUPPORT;
	_ret = -1;
	func = "socket";		// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = sock = socket( -666, SOCK_STREAM, 0);
	if (((ret != _ret || (errno != _err && errno != EPROTONOSUPPORT)) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	_err = EBADF;
	_ret = -1;
	func = "close";			// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = close( sock);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	_err = EINVAL;
	_ret = -1;
	func = "socket";		// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = sock = socket( PF_INET, -777, 0);
	if (((ret != _ret || (errno != _err && errno != EPROTONOSUPPORT)) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	_err = EPROTONOSUPPORT;
	_ret = -1;
	func = "socket";		// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = sock = socket( PF_INET, SOCK_STREAM, -666);
	if (((ret != _ret || (errno != _err && errno != ESOCKTNOSUPPORT)) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	_err = 0;
	_ret = 1;
	func = "socket";		// should work
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = sock = socket( PF_INET, SOCK_STREAM, 0);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	
	on = 1;
	_err = EBADF;
	_ret = -1;
	func = "setsockopt";		// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = setsockopt( -666, SOL_SOCKET, SO_LINGER, (const void *)&on, sizeof( on));
	if (((ret != _ret || (errno != _err && errno != ENOTSOCK)) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}

	on = 1;
	_err = ENOTSOCK;
	_ret = -1;
	func = "setsockopt";		// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = setsockopt( 0, SOL_SOCKET, SO_LINGER, (const void *)&on, sizeof( on));
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}

	on = 1;
	_err = ENOPROTOOPT;
	_ret = -1;
	func = "setsockopt";		// should fail
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = setsockopt( sock, -666, SO_LINGER, (const void *)&on, sizeof( on));
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}

	on = 1;
	_err = 0;
	_ret = 0;
	func = "setsockopt";		// should work
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = setsockopt( sock, SOL_SOCKET, SO_LINGER, (const void *)&on, sizeof( on));
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}

	len = sizeof( sa);
	memset( &sa, 0, len);
	sa.sin_family = AF_INET;
	sa.sin_port = htons( 65534);		// should not exist
	sa.sin_addr.s_addr = inet_addr( "127.0.0.1");
	_err = ECONNREFUSED;
	_ret = -1;
	func = "connect";
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = connect( sock, (const struct sockaddr *)&sa, len);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	sa.sin_family = AF_INET;
	sa.sin_port = htons( 30000);		// should exist
	sa.sin_addr.s_addr = inet_addr( "127.0.0.1");
	_err = 0;
	_ret = 0;
	func = "connect";
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = connect( sock, (const struct sockaddr *)&sa, len);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	snprintf( buf, sizeof( buf), "test write\n");
	_err = 0;
	_ret = 0;
	func = "write";
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	n = write( sock, buf, strlen( buf));
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
	snprintf( buf, sizeof( buf), "test send\n");
	_err = 0;
	_ret = 0;
	func = "send";
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	n = send( sock, buf, strlen( buf), 0);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}
/*	n = read();
	n = send();
	n = recv();*/
	_err = 0;
	_ret = 0;
	func = "close";		// should work
	result = __LINE__;
	printf( "Testing '%s' %s: ", func, _ret < 0 ? "failure" : "success");fflush( stdout);
	ret = close( sock);
	if (((ret != _ret || errno != _err) && _ret < 0) || (!_ret && !_err && ret))
	{
		printf( "FAIL : ret=%d, should be %d; errno=%d, should be %d (%s)\n", ret, _ret, errno, _err, strerror( _err));
		perror( func);
		goto err;
	}
	else
	{
		printf( "SUCCESS : %s : ret=%d errno=%d (%s)\n", func, ret, _err, strerror( _err));fflush( stdout);
	}

	result = 0;
err:

	return result;
}


