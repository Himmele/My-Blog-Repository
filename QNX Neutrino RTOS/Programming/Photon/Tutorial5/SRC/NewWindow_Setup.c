/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"


int WindowCounter = 0;
PtWidget_t* Windows[5];

int
NewWindow_Setup( PtWidget_t *link_instance, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	char buffer[64];
	
	/* eliminate 'unreferenced' warnings */
	link_instance = link_instance, apinfo = apinfo, cbinfo = cbinfo;
	
	if(WindowCounter == 5)
		return(Pt_END);		// returning Pt_END tells PhAB to destroy the module without realizing it
	
	Windows[WindowCounter] = link_instance;
	
	sprintf(buffer, "Window %d", WindowCounter + 1);
	PtSetResource(Windows[WindowCounter], Pt_ARG_WINDOW_TITLE, buffer, 0);
	WindowCounter++;

	return( Pt_CONTINUE );

	}

