#include <sys/neutrino.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>

#define ND_LOCAL_NODE 0

void SignalHandler(signo)
{
	printf("signal handler\n");
}

int main()
{
	 extern void SignalHandler();
	 struct sigaction act;
	 
	 act.sa_flags = 0;
     act.sa_handler = &SignalHandler;
   
	 //sigaction( SIGUSR1, &act, NULL );
	 SignalAction(NULL, __signalstub, SIGUSR1, &act, NULL);
	 
	 //kill( getpid(), SIGUSR1 );
	 SignalKill(ND_LOCAL_NODE, NULL, NULL, SIGUSR1, NULL, NULL);
	 
	 sleep(3);
	 printf("normal program termination\n");
	 
	 return 0;
}