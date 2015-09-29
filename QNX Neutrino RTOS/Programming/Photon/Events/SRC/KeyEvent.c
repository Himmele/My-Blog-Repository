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


int
KeyEvent( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	PhKeyEvent_t* KeyEvent = cbinfo->cbdata;
	unsigned long flags;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	if(cbinfo->event->type == Ph_EV_KEY)
		PtMessageBox(NULL, "key pressed", "key pressed", NULL, NULL);
	
	return( Pt_CONTINUE );

	}

