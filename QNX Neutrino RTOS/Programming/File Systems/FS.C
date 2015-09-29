#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int console, file;
	
	console = open("/dev/tty", O_WRONLY);
	write(console, "QNX\n", 4);
	lseek(console, 11, SEEK_CUR);									// not supported by a tty device
	write(console, "QNX\n", 4);
	
	if((file = open("MyFile", O_CREAT | O_RDWR)) == -1)
		printf("failed\n");
	write(file, "123", 3);
	lseek(file, 11, SEEK_CUR);
	write(file, "123", 3);
	
	link("MyFile", "MyFile2");											// create a hard link
	
	return 0;
}