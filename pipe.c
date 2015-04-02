#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <sys/time.h>
#include <sys/types.h>

int pfd[2] = { -1, -1 };
#define READ 0
#define WRITE 1

int end = 0;

void * task( void * opaque)
{
	void *result = 0;
	int max;
	int fd = pfd[READ];
	unsigned long id = (unsigned long)pthread_self();

	max = fd + 1;
	while (!end && fd >= 0)
	{
		int n;
		char buf[10];
		fd_set rfds;
		struct timeval tv;

		FD_ZERO( &rfds);
		FD_SET( fd, &rfds);
		memset( &tv, 0, sizeof( tv));
		tv.tv_sec = 0;
		tv.tv_usec = 10000;
		n = select( max, &rfds, NULL, NULL, &tv);
		if (n != 1)
			continue;

		n = read( fd, buf, sizeof( buf));
		if (n <= 0)
			continue;

		printf( "[%08lx]read returned %d\n", id, n);fflush( stdout);
	}

	printf( "[%08lx]returning\n", id);fflush( stdout);
	return result;
}

int main()
{
	int i, n;
	char buf[20];
	pthread_t tid;

	if (pipe( pfd) == -1)
		perror( "pipe");
	else
	{
		printf( "succesfully create pipe : %d,%d\n", pfd[0], pfd[1]);fflush( stdout);
	}

	if (pthread_create( &tid, NULL, task, NULL))
		perror( "pthread_create");
	else
	{
		printf( "succesfully create thread : [%08lx]\n", (unsigned long)tid);fflush( stdout);
	}

	for (i = 0; i < 10; i++)
	{
		usleep( 100000);
	}
	snprintf( buf, sizeof( buf), "hello pipe\n");
	n = strlen( buf);
	printf( "about to write %d bytes\n", n);
	write( pfd[WRITE], buf, n);
	for (i = 0; i < 10; i++)
	{
		usleep( 100000);
	}
	printf( "signalling end..\n");fflush( stdout);
	end = 1;

	if (pthread_join( tid, NULL))
		perror( "pthread_join");
	else
	{
		printf( "succesfully joined thread\n");fflush( stdout);
	}

	printf( "The end.\n");fflush( stdout);

	return 0;
}

