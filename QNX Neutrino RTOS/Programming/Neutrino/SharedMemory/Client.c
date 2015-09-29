#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/neutrino.h>

int main( int argc, char** argv )
{
    int fd;
    unsigned *addr;
    int ConnectionID;

    /* Create a new memory object */
    fd = shm_open( "/MySharedMemory", O_RDWR, 0777 );
    if( fd == -1 ) 
    {
        printf("error: couldn't open a shared memory object\n");
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

    if((ConnectionID = ConnectAttach(0, addr[1022], addr[1023], _NTO_SIDE_CHANNEL, 0)) == -1)
    		printf("error: couldn't attach to channel\n");
    
    while(1)
    {
    		/* synchronize with the server: block until server has written shared memory */
    		if(MsgSend(ConnectionID, NULL, 0, NULL, 0) == -1)
    			break;
 
    		/* Read to shared memory */
    		printf("read shared memory: %s\n", (char*)addr);
    		sleep(1);
    	}

    close( fd );
    shm_unlink( "/MySharedMemory" );

    return EXIT_SUCCESS;
}