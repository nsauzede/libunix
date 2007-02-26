#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main( int argc, char *argv[])
{
	int err = 0;

	if (argc == 2)
	{
		sscanf( argv[1], "%d", &err);
		printf( "err code %d : %s\n", err, strerror( err));
	}
	else
		printf( "Usage: %s err_code\n\n", argv[0]);

	return 0;
}

