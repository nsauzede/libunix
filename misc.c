#include <stdio.h>
#include <unistd.h>

int main()
{
	printf( "sleeping 1sec..\n");fflush( stdout);
	sleep( 1);
	printf( "finished!\n");fflush( stdout);

	return 0;
}

