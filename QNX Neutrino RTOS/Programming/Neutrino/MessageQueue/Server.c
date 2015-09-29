// this message queue uses the POSIX NONBLOCKING IPC primitives

#include <stdio.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>


int main()
{
	int MessageQueueID;
	char MessageBuffer[1024];
	int counter;
	
	/* create a message queue in /dev/mqueue */
	if((MessageQueueID = mq_open("/MyMessageQueue", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, NULL)) == -1)
		printf("error: couldn't create the POSIX message queue\n"); 
	
	for(counter = 0; counter < 7; counter++)
	{
		if((counter % 2 ) == 0)
			sprintf(MessageBuffer, "a message for you");
		else
			sprintf(MessageBuffer, "another message for you");
		mq_send(MessageQueueID, MessageBuffer, 1024, 0);							// send a new message into the message queue
	}

	mq_close(MessageQueueID);
	
	return 0;
}