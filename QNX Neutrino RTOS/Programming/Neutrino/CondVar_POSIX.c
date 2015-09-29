#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int condition = 0;
int count = 0;

int consume( void )
{
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 0 )
         pthread_cond_wait( &cond, &mutex );
      printf( "Consumed %d\n", count );
      condition = 0;
      pthread_cond_signal( &cond );      
      pthread_mutex_unlock( &mutex );
      
      sleep(1);
   }

   return( 0 );
}

void*  produce( void * arg )
{
   while( 1 )
   {
      pthread_mutex_lock( &mutex );
      while( condition == 1 )
         pthread_cond_wait( &cond, &mutex );
      printf( "Produced %d\n", count++ );
      condition = 1;
      pthread_cond_broadcast( &cond );      
      pthread_mutex_unlock( &mutex );
      
      sleep(1);
   }
   return( 0 );
}

int main( void )
{
   pthread_create( NULL, NULL, &produce, NULL );
   return consume();
}