#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <process.h>

int main( int argc, char** argv )
{
    int fd;
    unsigned *addr;
    int ChannelID, SenderID;
    int counter = 0;

    /* Create a new memory object */
    fd = shm_open( "/MySharedMemory", O_RDWR | O_CREAT, 0777 );
    if( fd == -1 ) 
    {
        printf("error: couldn't create a shared memory object\n");
        return EXIT_FAILURE;
    }
    
    /* Set the memory object's size */
    if( ftruncate( fd, 1024) == -1 )
    {
        printf("error: couldn't set the size\n");
        return EXIT_FAILURE;
    }

    /* Map the memory object */
    addr = mmap( 0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if( addr == MAP_FAILED ) 
    {
    	    printf("error: mapping in the memory failed\n");
    	    return EXIT_FAILURE;
    }
	
	printf( "memory mapping address is 0x%08x\n", addr );
    
	ChannelID = ChannelCreate(NULL);
	*(addr + 1022) = getpid();													// needed by client to set up connection for synchronization
	*(addr + 1023) = ChannelID;
	
	while(1)
	{
		SenderID = MsgReceive(ChannelID, NULL, 0, NULL);			// block until client blocks

    		/* Write to shared memory */
    		sprintf((char*)addr, "here is message number %d\n", counter++);
		MsgReply(SenderID, 0, NULL, 0);										// awake client to read shared memory
	}
	
    close( fd );
    shm_unlink( "/MySharedMemory" );

    return EXIT_SUCCESS;
}