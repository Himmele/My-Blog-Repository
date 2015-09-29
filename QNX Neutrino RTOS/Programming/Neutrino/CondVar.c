#include <stdio.h>
#include <pthread.h>

sync_t mutex;
sync_t cond;
int condition = 0;
int count = 0;

int consume( void )
{
   while( 1 )
   {
      SyncMutexLock( &mutex );
      while( condition == 0 )
         SyncCondvarWait( &cond, &mutex );
      printf( "Consumed %d\n", count );
      condition = 0;
      SyncCondvarSignal( &cond );      
      SyncMutexUnlock( &mutex );
      
      sleep(1);
   }

   return( 0 );
}

void*  produce( void * arg )
{
   while( 1 )
   {
      SyncMutexLock( &mutex );
      while( condition == 1 )
         SyncCondvarWait( &cond, &mutex );
      printf( "Produced %d\n", count++ );
      condition = 1;
      SyncCondvarSignal( &cond );      
      SyncMutexUnlock( &mutex );
      
      sleep(1);
   }
   return( 0 );
}

int main( void )
{
   SyncTypeCreate(_NTO_SYNC_MUTEX_FREE, &mutex, NULL);
   SyncTypeCreate(_NTO_SYNC_COND, &cond, NULL);
   
   pthread_create( NULL, NULL, &produce, NULL );
   return consume();
}