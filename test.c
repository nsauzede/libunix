#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int var = 0;
pthread_mutex_t lock;

int end = 0;

typedef struct data_t {
	int delta;
	unsigned int id;
}_data_t;
struct data_t data[2];

#ifdef WIN32
//#define usleep(foo) _sleep(foo/1000)
//#define usleep(foo) Sleep(foo/1000)
#define rand_r(foo) rand()
#endif

#define R (100000)

#define LOOPS 30

void *taskcli( void *opaque)
{
	void *result = NULL;
	int sock = (int)(unsigned long)opaque;
	unsigned long id = (unsigned long)pthread_self();

	printf( "[%08lx]++managing sock=%d\n", id, sock);fflush( stdout);
	while (!end)
	{
		int _var;
		char buf[1000];
						
		pthread_mutex_lock( &lock);
		_var = var;
		pthread_mutex_unlock( &lock);
		snprintf( buf, sizeof( buf), "[%08lx]++var=%d\r\n", id, _var);
//		send( sock, buf, strlen( buf) + 1, 0);
		send( sock, buf, strlen( buf), 0);
//		printf( buf);fflush( stdout);
		usleep( 200000);
	}
	close( sock);

	printf( "[%08lx]++returning (closed socket %d)\n", id, sock);fflush( stdout);
	return result;
}

void *taskser( void *opaque)
{
	void *result = NULL;
	int sock;
	struct sockaddr_in sa;
	int port = 40000;
	int on;
	int max;
	struct timeval tv;
	unsigned long id = (unsigned long)pthread_self();
	
	memset( &tv, 0, sizeof( tv));
	sock = socket( PF_INET, SOCK_STREAM, 0);
	memset( &sa, 0, sizeof( sa));
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons( port);
	sa.sin_family = AF_INET;
	on = 1;
	setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&on, sizeof( on));
	bind( sock, (struct sockaddr *)&sa, sizeof( sa));
	listen( sock, 1);
	max = sock + 1;
	printf( "[%08lx]--accepting on port %d; sock=%d\n", id, port, sock);fflush( stdout);
	while (!end)
	{
		int cs;
		socklen_t len;
		pthread_t tid;
		fd_set rfds;

		FD_ZERO( &rfds);
		FD_SET( sock, &rfds);
		tv.tv_sec = 1;
		tv.tv_usec = 100000;
//		printf( "--selecting\n");fflush( stdout);
		if (select( max, &rfds, NULL, NULL, &tv) <= 0)
			continue;
//		printf( "--selected\n");fflush( stdout);
		len = sizeof( sa);
		cs = accept( sock, (struct sockaddr *)&sa, &len);
		if (cs < 0)
			continue;
//		printf( "--accepted sock=%d\n", cs);
		if (!pthread_create( &tid, NULL, taskcli, (void *)(long int)cs))
			printf( "--succesfully created thread\n");
		else
			perror( "--pthread_create");
//		close( cs);
	}
	close( sock);

	printf( "[%08lx]++returning (closed socket %d)\n", id, sock);fflush( stdout);
	return result;
}

void *task( void *opaque)
{
	void *result = NULL;
	int delta = ((int)(unsigned long)opaque) ? 1 : -1;
	unsigned int seed;
	unsigned long id = (unsigned long)pthread_self();

	pthread_mutex_lock( &lock);
#ifdef WIN32
	srand( time( 0) + id);
#endif
	printf( "[%08lx] delta=%d\n", id, delta);fflush(stdout);
	pthread_mutex_unlock( &lock);
	seed = time( 0) + id;
#ifdef WIN32
	seed = seed;			// on WIN32, rand_r is defined to rand, so this var is unused
#endif
	while (!end)
	{
		unsigned long t;

		t = rand_r( &seed) % R;
		usleep( t);
		pthread_mutex_lock( &lock);
		var += delta;
//		printf( "[%08lx] delta=%d t=%u\n", id, delta, (unsigned int)t);fflush(stdout);
//		printf( "%c", delta > 0 ? '+' : '-');fflush(stdout);
		pthread_mutex_unlock( &lock);
	}

	printf( "[%08lx]++returning\n", id);fflush( stdout);
	return result;
}

int pfd[2] = { -1, -1 };
#define READ 0
#define WRITE 1

void *taskpipe( void *opaque)
{
	void *result = NULL;
	int max = 0;
	struct timeval tv;
	unsigned long id = (unsigned long)pthread_self();
	
	memset( &tv, 0, sizeof( tv));
	max = pfd[READ] + 1;
	printf( "[%08lx] pipe=%d\n", id, pfd[READ]);fflush(stdout);
	while (!end && pfd[READ] >= 0)
	{
		char buf[10];
		int n;
		fd_set rfds;

		FD_ZERO( &rfds);
		FD_SET( pfd[READ], &rfds);
		tv.tv_sec = 1;
		tv.tv_usec = 100000;
//		printf( "[%08lx]selecting\n", id);fflush( stdout);
		if ((n = select( max, &rfds, NULL, NULL, &tv)) <= 0)
//		if ((n = select( max, &rfds, NULL, NULL, &tv)) != 1)
			continue;

		printf( "[%08lx]selected!!\n", id);fflush(stdout);
		if (FD_ISSET( pfd[READ], &rfds))
		{
			printf( "[%08lx]reading\n", id);fflush( stdout);
			n = read( pfd[READ], buf, sizeof( buf));
			if (n > 0)
			{
				printf( "[%08lx] read pipe n=%d\n", id, n);fflush( stdout);
			}
		}
	}
	printf( "[%08lx]++returning\n", id);fflush( stdout);

	return result;
}

int main()
{
	int i;
	pthread_t tid[4];
	int nt = 0;

	if (pthread_mutex_init( &lock, NULL))
		printf( "mutex NOT created\n");
	else
		printf( "mutex succesfully created\n");

	nt = 0;
	if (pthread_create( &tid[nt++], NULL, task, (void *)0))
		printf( "failed to create thread%d\n", nt);
	else
		printf( "succesfully created thread%d\n", nt);
	printf( "tid%d=%08lx\n", nt, (unsigned long)tid[nt-1]);

	if (pthread_create( &tid[nt++], NULL, task, (void *)1))
		printf( "failed to create thread%d\n", nt);
	else
		printf( "succesfully created thread%d\n", nt);
	printf( "tid%d=%08lx\n", nt, (unsigned long)tid[nt-1]);

	if (pthread_create( &tid[nt++], NULL, taskser, (void *)0))
		printf( "failed to create thread%d\n", nt);
	else
		printf( "succesfully created thread%d\n", nt);
	printf( "tid%d=%08lx\n", nt, (unsigned long)tid[nt-1]);

	if (pipe(pfd) == -1)
		perror( "pipe");
	else
		printf( "succesfully created pipe : %d,%d\n", pfd[0], pfd[1]);

	if (pthread_create( &tid[nt++], NULL, taskpipe, (void *)0))
		printf( "failed to create thread%d\n", nt);
	else
		printf( "succesfully created thread%d\n", nt);
	printf( "tid%d=%08lx\n", nt, (unsigned long)tid[nt-1]);

	if (pfd[WRITE] >= 0)
	{
		char buf[20];
		int n;

		snprintf( buf, sizeof( buf), "hello pipe\n");
		n = strlen( buf);
		printf( "About to write %d bytes to pipe..\n", n);fflush( stdout);
		do {
		n = write( pfd[WRITE], buf, n);
		if (n > 0)
		{
			printf( "wrote %d bytes\n", n);fflush( stdout);
		}
		else
			perror( "write");
		} while (n < 0);
	}
	for (i = 0; i < LOOPS; i++)
	{
		usleep( 200000);
	}
	printf( "Signaling the end..\n");fflush(stdout);
	end = 1;

#if 0
	for (i = 0; i < 10*LOOPS; i++)
	{
		usleep( 200000);
	}
#endif

	for (i = 0; i < nt; i++)
	{
		printf( "Joining%d..\n", i + 1);fflush( stdout);
		if (pthread_join( tid[i], NULL))
			printf( "failed to join thread%d\n", i + 1);
		else
			printf( "thread%d succesfully joined\n", i + 1);
	}

	pthread_mutex_destroy( &lock);

	printf( "The End.\n");fflush( stdout);
	return 0;
}

