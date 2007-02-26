#include <stdio.h>

int main()
{
	int sock;
	struct sockaddr_in sa;

	sock = socket( PF_INET, SOCK_STREAM, 0);
	memset( &sa, 0, sizeof( sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons( 30000);
	sa.sin_addr = inet_addr( "127.0.0.1");

	return 0;
}

