// this message queue uses the POSIX NONBLOCKING IPC primitives

#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>


int main()
{
	int MessageQueueID;
	char MessageBuffer[4096];
	
	if((MessageQueueID = mq_open("/MyMessageQueue", O_RDWR, NULL, NULL)) == -1)		// open an existing message queue
		printf("error: couldn't open the POSIX message queue\n"); 
	
	while(1)
	{
		/* mq_receive always returns one message */
		mq_receive(MessageQueueID, MessageBuffer, 4096, 0);												// the size file MUST be mq_msgsize
		printf("received message: %s\n", MessageBuffer);
	}

	return 0;
}