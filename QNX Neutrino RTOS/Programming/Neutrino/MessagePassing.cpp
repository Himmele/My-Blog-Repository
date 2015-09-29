// compile using: qcc Messages.cpp

#include <sys/neutrino.h>
#include <stdio.h>
#include <unistd.h>


int ChannelID;

void* SenderThread(void* args)
{
	char SenderBuffer[128];
	char ReplyBuffer[32];
	int ConnectionID;
	
	ConnectionID = ConnectAttach(0, 0, ChannelID, 0 | _NTO_SIDE_CHANNEL, NULL);		// attach to the channel
	if(ConnectionID == -1)
		printf("error: connecting to channel\n");

	sprintf(SenderBuffer, "a message for you");
	if(MsgSend(ConnectionID, SenderBuffer, 128, ReplyBuffer, 32) == -1)			// send a message through the attached connection
		printf("error: sending the message\n");														// and wait for a reply
	
	printf("got reply: %s\n", ReplyBuffer);
		
	return NULL;
}

void* ReceiverThread(void*args)
{
	char ReceiverBuffer[128];
	char Reply[32];
	int Sender;
	
	sprintf(Reply, "here is the reply");
	ChannelID = ChannelCreate(NULL);																// set up a new channel for message passing
	if(ChannelID == -1)
		printf("error: ChannelID\n");
	
	while(1)
	{
		Sender = MsgReceive(ChannelID, ReceiverBuffer, 128, NULL);					// do a receive on the newly created channel
		if(Sender == -1)
			printf("error: receiving the message\n");
		
		MsgReply(Sender, 0, Reply, 32);																// send a reply in turn to a send-message
		printf("received message: %s\n", ReceiverBuffer);
		
		break;
	}
	
	return NULL;
}

int main()
{
	ThreadCreate(NULL, ReceiverThread, NULL, NULL);
	ThreadCreate(NULL, SenderThread, NULL, NULL);
	sleep(3);
	
	return 0;
}