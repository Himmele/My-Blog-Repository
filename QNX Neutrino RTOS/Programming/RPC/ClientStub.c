// 1. gcc -fPIC -c ClientStub.c
// 2. gcc -shared -o ClientStub.so ClientStub.o

#include <sys/neutrino.h>
#include <unistd.h>

int PID, ChannelID;

void Initialize()
{
	printf("PID: ");
	scanf("%d", &PID);
	printf("ChannelID: ");
	scanf("%d", &ChannelID);
}

int GetVersion()
{
	int ConnectionID;
	int Version = 0;
	
	iov_t RPC[2];
	char RPCType[16];
	SETIOV(&RPC[0], &RPCType, sizeof(RPCType));
	sprintf(RPCType, "VERSION");
			
	ConnectionID = ConnectAttach(0, PID, ChannelID, 0, 0);
	MsgSendvs(ConnectionID, RPC, 1, &Version, sizeof(Version));
	ConnectDetach(ConnectionID);
	
	return Version;
}

int GetSquare(int figure)
{
	int ConnectionID;
	int Square = 0;
	
	iov_t RPC[2];
	char RPCType[16];
	SETIOV(&RPC[0], &RPCType, sizeof(RPCType));
	SETIOV(&RPC[1], &figure, sizeof(figure));
	sprintf(RPCType, "SQUARE");
			
	ConnectionID = ConnectAttach(0, PID, ChannelID, 0, 0);
	MsgSendvs(ConnectionID, RPC, 2, &Square, sizeof(Square));
	ConnectDetach(ConnectionID);
	
	return Square;
}