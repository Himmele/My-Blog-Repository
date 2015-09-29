#include <stdio.h>
#include <pthread.h>

sync_t mutex;
int condition = 0;
int count = 0;
int flag = 0;

int consume( void )
{
   while( 1 )
   {
      SyncMutexLock( &mutex );			// possible starvation
      if( condition == 0 )
      {
         if(flag == 0)
         	printf("starvation\n");
         flag = 0;
         SyncMutexUnlock(&mutex);
         continue;
      }
      printf( "Consumed %d\n", count );
      condition = 0;   
      SyncMutexUnlock( &mutex );
      
      sleep(1);
   }

   return( 0 );
}

void*  produce( void * arg )
{
   while( 1 )
   {
      SyncMutexLock( &mutex );			// possible starvation
      if( condition == 1 )
      {
         if(flag == 1)
         	printf("starvation\n");
         flag = 1;
         SyncMutexUnlock(&mutex);
         continue;
      }
      printf( "Produced %d\n", count++ );
      condition = 1;     
      SyncMutexUnlock( &mutex );
      
      sleep(2);
   }
   return( 0 );
}

int main( void )
{
   SyncTypeCreate(_NTO_SYNC_MUTEX_FREE, &mutex, NULL);
   
   pthread_create( NULL, NULL, &produce, NULL );
   return consume();
}