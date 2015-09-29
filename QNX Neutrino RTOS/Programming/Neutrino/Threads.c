#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void*  ThreadFunction( void*  arg )
{
   printf( "%s%d\n", (char*) arg, pthread_self() );
   
   if(pthread_self() != 2)
      ThreadJoin(2, NULL);			// block until thread 2 terminates
    
   // ThreadDestroy(0, -1, 0);		// destroy current thread
   
   if(pthread_self() == 2)
      sleep(5);
   else
      sleep(1);
      
   printf("leaving: %d\n", pthread_self());
   
   return( 0 );
}

int main( void )
{
   pthread_attr_t attr;
   
   pthread_attr_init( &attr );
   pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );

   pthread_create( NULL, &attr, &ThreadFunction, "Thread ID: " );
   
   ThreadCreate(NULL, ThreadFunction, "Thread ID: ", &attr);
   ThreadCreate(NULL, ThreadFunction, "Thread ID: ", NULL);
   
   // ThreadDestroy(0, 0, 0);	// destroy main thread
   
   sleep(11);
   return EXIT_SUCCESS;
}