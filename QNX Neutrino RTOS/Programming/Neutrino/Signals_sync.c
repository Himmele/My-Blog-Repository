// synchronous notification using the sigwaitinfo function of SignalWaitinfo kernel call
// use instead of pulses to avoid the overhead of setting up a channel and a connection

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/neutrino.h>


void SignalHandler()
{
	printf("signal handler\n");
}

void* SendSignal(void* args)
{
	sleep(3);
	// kill(getpid(), SIGUSR1);											// send SIGUSR1 to main thread of current process
	SignalKill(0, getpid(), 1, SIGUSR1, 0, 65536);				// send SIGUSR1 to main thread of current process
	
	return NULL;
}

int main()
{
	sigset_t set;
	siginfo_t info;
	struct sigaction action;
	
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	
	action.sa_flags = 0;
	action.sa_handler = SignalHandler;
	sigaction(SIGUSR1, &action, NULL);							// signal handler is not executed because of sigwaitinfo
	
	ThreadCreate(NULL, SendSignal, NULL, NULL);
	
	printf("waiting for signal\n");
	// sigwaitinfo(&set, &info);										// block and wait for the SIGUSR1 signal
	SignalWaitinfo(&set, &info);										// block and wait for the SIGUSR1 signal
	
	printf("received signal %d with code %d and value %d\n", info.si_signo, info.si_code, info.si_value.sival_int);
	
	sleep(5);
	return 0;
}