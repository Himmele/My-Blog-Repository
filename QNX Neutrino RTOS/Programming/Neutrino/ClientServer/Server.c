// to avoid deadlocks, you should arrange your threads in a hierarchy, with sends going up the tree
// thus in a client-server model, the client always sends messages to the server, which only replies
// !!! what is to do, if the server has to tell something to the client? !!!
// solution: the client first sends the server a PULSE-EVENT, which is asynchronously passed back to the client when desired

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/iomsg.h>
#include <sys/dispatch.h>

#include "Definitions.h"

int main( int argc, char **argv)
{
	int rcvid;
	struct my_msg msg;
	name_attach_t *attach;

	/* attach the name the client will use to find us */
	/* our channel will be in the attach structure */
	if ( (attach = name_attach( NULL, MY_SERV, 0 )) == NULL)					// register a name in the namespace and create a channel
	{
     	printf("server: failed to attach name, errno: %d\n", errno );
     	exit(1);
    }
  
	/* wait for the message from the client */
	rcvid = MsgReceive( attach->chid, &msg, sizeof( msg ), NULL );
	if ( msg.type == MSG_GIVE_PULSE )
	{ 
     	/* wait until it is time to notify the client */
     	sleep(2);

     	/* deliver notification to client that client requested */
     	MsgDeliverEvent( rcvid, &msg.event );
     	printf("server: delivered event\n");
	} else
  	{
    		printf("server: unexpected message\n");
  	}
  
	return 0;
}