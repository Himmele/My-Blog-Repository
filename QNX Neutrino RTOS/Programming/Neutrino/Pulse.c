#include <sys/neutrino.h>


int ChannelID;

void* SenderThread(void* args)
{
	int ConnectionID;
	
	ConnectionID = ConnectAttach(0, 0, ChannelID, 0, 0);															// attach to the channel
	MsgSendPulse(ConnectionID, sched_get_priority_max(SCHED_RR), 127, 2147483647);		// send a pulse
	
	return NULL;
}

void* ReceiverThread(void* args)
{
	int SenderID;
	struct _pulse Buffer;																										// _pulse structure: see neutrino.h
	
	ChannelID = ChannelCreate(NULL);																				// create a channel
	SenderID = MsgReceive(ChannelID, &Buffer, sizeof(Buffer), NULL);								// receive the pulse and store it in Buffer
	
	printf("SenderID: %d\n", SenderID);
	printf("received pulse: %d, %d\n", Buffer.code, Buffer.value);
			
	return NULL;
}


int main()
{
	ThreadCreate(NULL, ReceiverThread, NULL, NULL);
	ThreadCreate(NULL, SenderThread, NULL, NULL);
	
	sleep(3);
	
	return 0;
}