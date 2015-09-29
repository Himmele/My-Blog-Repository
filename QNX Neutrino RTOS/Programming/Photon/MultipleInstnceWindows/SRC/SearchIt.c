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
SearchIt( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	char** Text;
	PtWidget_t *Window, *TextString;
	
	Window = ApGetInstance(widget);
	TextString = ApGetWidgetPtr(Window, ABN_TextString);
	
	PtGetResource(TextString, Pt_ARG_TEXT_STRING, Text, 0);
	PtMessageBox(NULL, "you typed", *Text, NULL, NULL);
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	return( Pt_CONTINUE );

	}

