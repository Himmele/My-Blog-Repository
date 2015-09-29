// 1. gcc -c Server.c
// 2. gcc -o Server Server.o /root/ServerLib.so

#include <sys/neutrino.h>
#include <unistd.h>
#include <process.h>

int main()
{
	int SenderID;
	int ChannelID;
	int ReturnValue;
	int Figure = 0;
	
	iov_t RPC[2];
	char RPCType[16];
	SETIOV(&RPC[0], &RPCType, sizeof(RPCType));
	SETIOV(&RPC[1], &Figure, sizeof(Figure));
	
	ChannelID = ChannelCreate(NULL);
	printf("PID: %d\n", getpid());
	printf("ChannelID: %d\n", ChannelID);
	
	while(1)
	{
		SenderID = MsgReceivev(ChannelID, RPC, 2, NULL);
		
		printf("got RPC request\n");
		
		if(strcmp(RPCType, "VERSION") == 0)
			ReturnValue = version();
		if(strcmp(RPCType, "SQUARE") == 0)
			ReturnValue = square(Figure);
			
		MsgReply(SenderID, 0, &ReturnValue, sizeof(ReturnValue));
	}
	return 0;
}