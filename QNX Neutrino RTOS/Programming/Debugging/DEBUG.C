// gcc -g -o debug debug.c				--> -g: create debug symbols

// gdb commands
// 1. target qnx /dev/ser1,115200		--> remote debugging via pdebug running on target machine
// 2. upload debug /tmp/debug			--> upload the executable onto the target machine
// 3. sym debug								--> load the debug symbols
// 4. run /root/debug							--> run the executable
// 5. break main								--> set a breakpoint at main
// 6. c												--> continue up to the next breakpoint
// 7. n												--> next instruction
// 8. l													--> list the next set of instructions
// 9. ins x											--> inspect the contents of variable x
// 10. set x=3									--> assign a value to variable x
// 11. help											--> get the help main menu

int main()
{
	int x, y, z;
	
	printf("QNX\n");
	x = 3;
	y = 2;
	z = x * y;
	
	return 0;
}