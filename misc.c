#include <stdio.h>
#include <unistd.h>

int main()
{
	printf( "sleeping 1sec..\n");fflush( stdout);
	usleep( 1000000);
	printf( "finished!\n");fflush( stdout);
	printf( "sleeping 500msec..\n");fflush( stdout);
	usleep( 500000);
	printf( "finished!\n");fflush( stdout);

	return 0;
}

