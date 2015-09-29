#include <sys/neutrino.h>
#include <stdio.h>

int main()
{
	printf("starting\n");
	
	ThreadCtl(_NTO_TCTL_IO, 0);
	InterruptDisable();
	
	printf("interrupts disabled\n");
	sleep(11);
	
	InterruptEnable();
	
	printf("interrupts enabled\n");
	printf("done\n");

	return 0;
}