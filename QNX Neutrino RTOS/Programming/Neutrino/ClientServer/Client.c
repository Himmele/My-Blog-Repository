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

#include "Definitions.h"

int main( int argc, char **argv)
{
	int chid, coid, srv_coid, rcvid;
	struct my_msg msg;
	struct _pulse pulse;

	/* we need a channel to receive the pulse notification on */
	chid = ChannelCreate( 0 ); 

	/* and we need a connection to that channel for the pulse to be delivered on */
	coid = ConnectAttach( 0, 0, chid, _NTO_SIDE_CHANNEL, 0 );

	/* fill in the event structure for a pulse: coid is the connection on which the pulse is send back to the client*/
	SIGEV_PULSE_INIT( &msg.event, coid, SIGEV_PULSE_PRIO_INHERIT, MY_PULSE_CODE, 0 );
	msg.type = MSG_GIVE_PULSE;

	/* find the server */
	if ( (srv_coid = name_open( MY_SERV, 0 )) == -1)
	{
     	printf("failed to find server, errno: %d\n", errno );
     	exit(1);
  	}

	/* give the pulse event we initialized above to the server for later delivery */
	MsgSend( srv_coid, &msg, sizeof(msg), NULL, 0 );

	/* wait for the pulse from the server */
	rcvid = MsgReceivePulse( chid, &pulse, sizeof( pulse ), NULL );
	printf("got pulse with code %d\n", pulse.code);

	return 0;
}