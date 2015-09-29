#include <atomic.h>
#include <stdio.h>

volatile int counter = 0;

int main()
{
	atomic_add(&counter, 7);				// completes without being preempted by another thread, even in a SMP system
	printf("counter: %d\n", counter);
	
	return 0;
}