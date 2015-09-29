#include <sys/neutrino.h>
#include <stdio.h>


int main()
{
	SchedSet(NULL, NULL, SCHED_RR, NULL);		// schedules the current thread in round-robin order
	SchedYield();												// voluntarily gives away the CPU
	
	sched_setscheduler(NULL, SCHED_FIFO, NULL);	// schedules current process in FIFO order
	
	return 0;
}