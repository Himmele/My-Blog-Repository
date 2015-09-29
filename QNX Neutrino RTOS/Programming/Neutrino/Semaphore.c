#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/neutrino.h>


sync_t Semaphore;


void* Thread1 (void* args)
{
	printf("%d: entering\n", pthread_self());
	printf("%d: doing down on semaphore now\n", pthread_self());
	SyncSemWait(&Semaphore, NULL);
	printf("%d: unblocked from semaphore\n", pthread_self());
	
	return NULL;
}

void* Thread2 (void* args)
{
	printf("%d: entering\n", pthread_self());
	printf("%d: doing up on semaphore now\n", pthread_self());
	SyncSemPost(&Semaphore);
	SyncSemPost(&Semaphore);
	printf("%d: did up on semaphore\n", pthread_self());
	
	return NULL;
}

int main()
{
	SchedSet(NULL, NULL, SCHED_RR, NULL);
	SyncTypeCreate(_NTO_SYNC_SEM, &Semaphore, NULL);		// initial count: 0
	
	ThreadCreate(NULL, Thread1, NULL, NULL);
	ThreadCreate(NULL, Thread1, NULL, NULL);
	sleep(3);
	ThreadCreate(NULL, Thread2, NULL, NULL);
	SchedYield();
			
	sleep(11);
	return(0);
}