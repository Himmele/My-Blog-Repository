#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int main( void )
{
    sigset_t set;

    sigemptyset( &set );
    sigaddset( &set, SIGUSR1 );

	//kill( getpid(), SIGUSR1 );							// process is killed because signal is not blocked
	
	//SignalProcmask(NULL, NULL, SIG_BLOCK, &set, NULL);
	sigprocmask(SIG_BLOCK, &set, NULL);		// block signal
 
    kill( getpid(), SIGUSR1 );							// you cannot block SIGKILL or SIGSTOP

	printf("normal program termination\n");
	
    return EXIT_SUCCESS;
}