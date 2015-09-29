// Thread Local Storage

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_key_t key;

void key_destruct( void *value )
{
    free( value );
    pthread_setspecific( key, NULL );
}

char *lookup( void )
{
    char *string;

    string = (char *)pthread_getspecific( key );
    if( string == NULL ) 
    {
        string = (char *) malloc( 32 );
        sprintf( string, "This is thread: %d\n", pthread_self() );      
        pthread_setspecific( key, (void *)string );
    }

    return( string );
}

void *function( void *arg )
{
    while( 1 ) 
    {
        puts( lookup() );
    }

    return( 0 );
}

int main( void )
{
    pthread_key_create( &key, &key_destruct );
    pthread_create( NULL, NULL, &function, NULL );
	
    /* Let the threads run for 3 seconds. */
    sleep( 3 );
    
    return EXIT_SUCCESS;
}