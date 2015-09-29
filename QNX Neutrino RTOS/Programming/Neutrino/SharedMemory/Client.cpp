#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/neutrino.h>

int main( int argc, char** argv )
{
    int fd, FileDescriptor, FileSize;
    unsigned *addr;
    int ConnectionID;
    int counter = 0;
    
    /* Open a new file */
    FileDescriptor = open("Movie.mpg", O_RDWR | O_CREAT);

    /* Create a new memory object */
    fd = shm_open( "/MySharedMemory", O_RDWR, 0777 );
    if( fd == -1 ) 
    {
        printf("error: couldn't open a shared memory object\n");
        return 1;
    }
    
    /* Map the memory object to receive PID, ChannelID and File Size*/
    addr = (unsigned*) mmap( 0, 16, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if( addr == MAP_FAILED ) 
    {
    	    printf("error: mapping in the memory failed\n");
    	    return 1;
    }

    printf( "memory mapping address is 0x%08x\n", addr );

    if((ConnectionID = ConnectAttach(0, addr[0], addr[1], _NTO_SIDE_CHANNEL, 0)) == -1)
    		printf("error: couldn't attach to channel\n");
    	
    	FileSize = addr[2];
    	char* Buffer = new char [FileSize];
    
    	/* Unmap the memory object to map it in with the right size */
    	munmap(addr, 16);
    	
    	/* Map the memory object with the right size*/
    addr = (unsigned*) mmap( 0, FileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if( addr == MAP_FAILED ) 
    {
    	    printf("error: mapping in the memory failed\n");
    	    return 1;
    }
    
    while(1)
    {
    		/* synchronize with the server: block until server has written shared memory */
    		if(MsgSend(ConnectionID, NULL, 0, NULL, 0) == -1)
    			break;
 
    		/* Read to shared memory */
    		// write(FileDescriptor, (void*)addr, FileSize);				// this is also possible
    		printf("File Size: %d\n", FileSize);
    		memcpy(Buffer, addr, FileSize);									// but for performance measurement this approach is used
    		printf("memory copy complete\n");
    		break;
    	}
	
	write(FileDescriptor, Buffer, FileSize);
	printf("file written\n");
	
    close( fd );
    shm_unlink( "/MySharedMemory" );

    return 0;
}