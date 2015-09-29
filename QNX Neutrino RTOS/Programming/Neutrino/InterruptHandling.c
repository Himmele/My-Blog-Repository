#include <stdio.h>
#include <sys/neutrino.h>

struct sigevent event;
volatile unsigned counter = 0;

const struct sigevent* handler(void* area, int size)
{
	if(++counter == 10)
	{
		counter = 0;
		return(&event);																// return SIGEV_INTR after 11 invocations 
	}
	else
		return NULL;
}

int main()
{
	int id;
	struct _clockperiod period;
	
	period.nsec = 500000;
	period.fract = 0;
	ClockPeriod(CLOCK_REALTIME, &period, NULL, NULL);			// set the resolution of the clock
	
	event.sigev_notify = SIGEV_INTR;
	ThreadCtl( _NTO_TCTL_IO, 0 );												// control a thread to make QNX-specific changes to it
	
	/*
	* the interrupt redirector in the microkernel pushes the context of the current thread into the appropriate thread table *
	* and sets the processor context to the ISR's thread context. This allows the ISR to use buffers and code in the user level *
	* thread. The redirector code will call each ISR attached to that hardware interrupt *
	*/
	id = InterruptAttach(1, &handler, NULL, 0, 0);						// attach to the keyboard interrupt vector
	// id = InterruptAttach(0, &handler, NULL, 0, 0);						// attach to the clock interrupt vector
	
	InterruptWait(0, NULL);															// wait for an interrupt handler returning SIGEV_INTR
	printf("received 5 key presses and 5 key releases\n");
	
	InterruptDetach(id);
	exit(0);
	
	return 0;
}