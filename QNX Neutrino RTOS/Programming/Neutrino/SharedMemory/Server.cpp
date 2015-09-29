#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <process.h>
#include <sys/neutrino.h>

int main( int argc, char** argv )
{
    int fd, FileDescriptor, FileSize = 0;
    unsigned *addr;
    int ChannelID, SenderID;
    int counter = 0;
	
	/* Open a file for copying*/
	FileDescriptor = open("Goodtimes.mpg", O_RDONLY);
	FileSize = lseek(FileDescriptor, 0, SEEK_END);
	char* File = new char[FileSize];
	lseek(FileDescriptor, 0, SEEK_SET);
	read(FileDescriptor, File, FileSize);
	
    /* Create a new memory object */
    fd = shm_open( "/MySharedMemory", O_RDWR | O_CREAT, 0777 );
    if( fd == -1 ) 
    {
        printf("error: couldn't create a shared memory object\n");
        return 1;
    }
    
    /* Set the memory object's size */
    if( ftruncate( fd, FileSize) == -1 )
    {
        printf("error: couldn't set the size\n");
        return 1;
    }

    /* Map the memory object */
    addr = (unsigned*) mmap( 0, FileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0 );
    if( addr == MAP_FAILED ) 
    {
    	    printf("error: mapping in the memory failed\n");
    	    return 1;
    }
	
	printf( "memory mapping address is 0x%08x\n", addr );
    
	ChannelID = ChannelCreate(NULL);
	*(addr + 0) = getpid();															// needed by client to set up connection for synchronization
	*(addr + 1) = ChannelID;
	*(addr + 2) = FileSize;
	
	while(1)
	{
		SenderID = MsgReceive(ChannelID, NULL, 0, NULL);			// block until client blocks
    		/* Write to shared memory */
    		memcpy(addr, File, FileSize);
    	
		MsgReply(SenderID, 0, NULL, 0);										// awake client to read shared memory
	}
	
    close( fd );
    shm_unlink( "/MySharedMemory" );

    return 0;
}