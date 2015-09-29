#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/neutrino.h>


sync_t Mutex;


void* ThreadLock (void* args)
{
	printf("tid %d: locking mutex now\n", pthread_self());
	SyncMutexLock(&Mutex);
	printf("tid %d: operating after locking the mutex\n", pthread_self());
	
	sleep(5);
	printf("tid %d: unlocking mutex now\n", pthread_self());
	SyncMutexUnlock(&Mutex);										// the thread which locks a mutex MUST also unlock the mutex
	printf("tid %d: mutex unlocked\n", pthread_self());
			
	return NULL;
}

void* ThreadUnlock (void* args)
{
	printf("tid %d: unlocking mutex now\n", pthread_self());
	SyncMutexUnlock(&Mutex);
	printf("tid %d: mutex unlocked\n", pthread_self());
	
	return NULL;
}

int main()
{
	SchedSet(NULL, NULL, SCHED_RR, NULL);
	SyncTypeCreate(_NTO_SYNC_MUTEX_FREE, &Mutex, NULL);
	
	ThreadCreate(NULL, ThreadLock, NULL, NULL);			// locks mutex, sleeps, returns
	ThreadCreate(NULL, ThreadLock, NULL, NULL);			// locks mutex, blocks
	
	ThreadCreate(NULL, ThreadUnlock, NULL, NULL);		// unlocks mutex, returns (doesn't unblock thread 3)
	SchedYield();
	sleep(60);
	
	return(0);
}