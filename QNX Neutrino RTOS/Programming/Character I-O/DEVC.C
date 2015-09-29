#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
	int ethernet, serial, tty;
	struct termios terminal;
	char buf[11];
	
	if((serial = open("/dev/ser1", O_RDWR)) == -1)
		printf("cannot open serial device\n");
		
	if((write(serial, "Hello", 5)) == -1)
		printf("an error occured\n");
	
	if((ethernet = open("/dev/io-net/en0", O_RDWR)) == -1)
		printf("cannot open ethernet device\n");
	
	if((write(ethernet, "Hello", 5)) == -1)
		printf("an error occured\n");
	
	if((tty = open("/dev/tty", O_RDWR)) == -1)
		printf("cannot open tty device\n");
		
	write(tty, "Hello", 5);
	
	tcgetattr(tty, 	&terminal);
	if((terminal.c_iflag & ICRNL) == ICRNL)							// see termios.h for details
		printf("terminal: map CR to NL\n");
		
	return 0;
}