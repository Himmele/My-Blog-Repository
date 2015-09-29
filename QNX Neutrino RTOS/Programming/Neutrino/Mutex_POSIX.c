#include <stdio.h>
#include <sys/neutrino.h>
#include <stdlib.h>
#include <pthread.h>

#define EBUSY 16
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;		// PTHREAD_RMUTEX_INITIALIZER
int count = 0;

void* function1( void* arg )
{
    int tmp = 0;

    while( 1 ) {
        if(pthread_mutex_trylock(&mutex) == EBUSY)
        		printf("%d: mutex is already locked\n", pthread_self());
        pthread_mutex_lock( &mutex );
        tmp = count--;
        sleep(1);
        pthread_mutex_unlock( &mutex );
        printf( "%d: count is %d\n", pthread_self(), tmp );

        /* snooze for 2 seconds */
        sleep( 2 );
    }

    return 0;
}

void* function2( void* arg )
{
    int tmp = 0;

    while( 1 ) {
        if(pthread_mutex_trylock(&mutex) == EBUSY)
        		printf("%d: mutex is already locked\n", pthread_self());
        pthread_mutex_lock( &mutex );
        tmp = count++;
        pthread_mutex_unlock( &mutex );
        printf( "%d: count is %d\n", pthread_self(), tmp );

        /* snooze for 1 second */
        sleep( 1 );
    }

    return 0;
}

int main( void )
{
    pthread_create( NULL, NULL, &function1, NULL );
    pthread_create( NULL, NULL, &function2, NULL );

    /* Let the threads run for 60 seconds. */
    sleep( 60 );

    return 0;
}