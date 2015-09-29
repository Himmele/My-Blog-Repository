#include <sys/neutrino.h>
#include <unistd.h>


int ChannelID;


void*  SenderThread(void* args)
{
	int ConnectionID;
	iov_t MultipartMessage[3];																// struct iov_t{ void* base, size_t length}
	iov_t MultipartReply[2];
	char Buffer1[16], Buffer2[16], Buffer3[32];
	char Reply1[19], Reply2[19];
	
	SETIOV(&MultipartMessage[0], &Buffer1, sizeof(Buffer1));						// fill in the fields of an iov_t structure
	SETIOV(&MultipartMessage[1], &Buffer2, sizeof(Buffer2));
	SETIOV(&MultipartMessage[2], &Buffer3, sizeof(Buffer3));
	SETIOV(&MultipartReply[0], &Reply1, sizeof(Reply1));
	SETIOV(&MultipartReply[1], &Reply2, sizeof(Reply2));
	
	sprintf(Buffer1, "61 character st"); 
	Buffer1[15] = 'r';																						// overwrite the '\0' at the end
	sprintf(Buffer2, "ing to demonstr");
	Buffer2[15] = 'a';																						// overwrite the '\0' at the end
	sprintf(Buffer3, "te multipart message passing");
	
	ConnectionID = ConnectAttach(0, 0, ChannelID, 0, 0);								// attach to channel ChannelID
	MsgSendv(ConnectionID, MultipartMessage, 3, MultipartReply, 2);			// send a multipart-message consisting of 3 parts
																													// and provide 2 reply-message buffers
	
	Reply1[18] = '\0';																						// build up a string useable by printf
	printf("received reply: %st%s\n", Reply1, Reply2); 									// the 't' it the one within "split"
	return NULL;
}

void* ReceiverThread(void* args)
{
	int SenderID;
	iov_t MultipartMessage[1];																		// struct iov_t{ void* base, size_t length}
	iov_t MultipartReply[1];
	char Buffer1[128];
	char Reply1[64];
	
	SETIOV(&MultipartMessage[0], &Buffer1, sizeof(Buffer1));						// fill in the fields of an iov_t structure
	SETIOV(&MultipartReply[0], &Reply1, sizeof(Reply1));

	ChannelID = ChannelCreate(NULL);															// create a channel
	
	while(1)
	{
		SenderID = MsgReceivev(ChannelID, MultipartMessage, 1, NULL);		// provide 1 message buffer
		
		sprintf(Reply1, "this reply is split up into two parts");
		MsgReplyv(SenderID, 0, MultipartReply, 1);											// send a reply message: MsgSendv splits up this reply

		break;
	}
	
	printf("received message: %s\n", Buffer1);
	return NULL;
}

int main()
{
	ThreadCreate(NULL, ReceiverThread, NULL, NULL);
	ThreadCreate(NULL, SenderThread, NULL, NULL);
	
	sleep(3);
	
	return 0;
}