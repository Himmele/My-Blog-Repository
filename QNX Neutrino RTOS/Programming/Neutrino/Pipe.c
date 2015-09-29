#include <stdio.h>
#include <unistd.h>

int main()
{
	int fd[2];
	char OutBuffer[32];
	char InBuffer[32];
	
	sprintf(OutBuffer, "a message for you over a pipe");
	pipe(fd);
	
	write(fd[1], OutBuffer, sizeof(OutBuffer));
	read(fd[0], InBuffer, sizeof(InBuffer));
	
	printf("received on pipe: %s\n", InBuffer);
	return 0;
}