#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int console, disk, mbr, random, figure = 0;
	char buffer[512];
	
	console = open("/dev/tty", O_WRONLY);
	write(console, "Resource Manager\n", 17);
	
	if((mbr = open("mbr", O_CREAT | O_RDWR)) == -1)
		printf("failed\n");
	disk = open("/dev/hd0", O_RDONLY);
	read(disk, buffer, 512);
	write(mbr, buffer, 512);
	close(disk);
	close(mbr);
	
	random = open("/dev/random", O_RDONLY);
	read(random, &figure, 4);
	printf("random figure: %u\n", figure);
	
	return 0;
}