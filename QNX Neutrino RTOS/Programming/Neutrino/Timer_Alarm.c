#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/neutrino.h>

void Timer()
{
	printf("timer expired\n");
}	

int main()
  {
    unsigned int  timeleft;
    struct _itimer timer;
    struct sigaction action;
    action.sa_flags = 0;
    action.sa_handler = Timer;
	sigaction( SIGALRM, &action, NULL );
	
	timer.nsec = 10000000000;															// 10000000000ns == 10s
	timer.interval_nsec = 0;
	
    printf( "set the alarm and sleep\n" );
    TimerAlarm(CLOCK_REALTIME, &timer, 0);										// Neutrino microkernel call
    //alarm( 10 );																					// POSIX call
    sleep( 5 );   /* go to sleep for 5 seconds */

    /*
     * To get the time left before the SIGALRM is 
     * to arrive, one must cancel the initial timer, 
     * which returns the amount of time it had 
     * remaining.
     */
    timeleft = alarm( 0 );
    printf( "time left before cancel: %d\n", timeleft );

    /*
     * Start a new timer that kicks us when timeleft
     * seconds have passed.
     */

    alarm( timeleft );

    /*
     * Wait until we receive the SIGALRM signal; any
     * signal kills us, though, since we don't have
     * a signal handler.
     */
    
    printf( "hanging around, waiting for alarm\n" );
    pause();
    return EXIT_SUCCESS;
  }