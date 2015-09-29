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

extern int WindowCounter;
extern PtWidget_t* Windows[5];

int
NewWindow_Close( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	PhWindowEvent_t* WindowEvent = cbinfo->cbdata;
	int i;

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	if(WindowEvent->event_f != Ph_WM_CLOSE);
		return( Pt_CONTINUE );
	
	for(i = 0; i < WindowCounter; i++)
	{
		if(Windows[i] == widget)
		{
			Windows[i] = NULL;
			break;
		}
	}
	return( Pt_CONTINUE );
	}

