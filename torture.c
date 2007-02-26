#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int result = 0;
	int sock;
	struct sockaddr_in sa;
	char buf[100];
	int n;
	socklen_t len;

	sock = socket( -666, -777, -888);
	if (sock == -1)
	{
		perror( "socket");
		result = -5;
		goto nosock;
	}
	sock = socket( PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		perror( "socket");
		result = -10;
		goto nosock;
	}
	len = sizeof( sa);
	memset( &sa, 0, len);
	sa.sin_family = AF_INET;
	sa.sin_port = htons( 30000);
	sa.sin_addr.s_addr = inet_addr( "127.0.0.1");
	if (connect( sock, (const struct sockaddr *)&sa, len) == -1)
	{
		perror( "connect");
		result = -20;
		goto nocon;
	}
	snprintf( buf, sizeof( buf), "test write\n");
	n = write( sock, buf, strlen( buf));
	if (n == -1)
	{
		perror( "write");
	}
	else
		printf( "write returned n=%d\n", n);
/*	n = read();
	n = send();
	n = recv();*/

nocon:
	close( sock);
nosock:

	return result;
}

