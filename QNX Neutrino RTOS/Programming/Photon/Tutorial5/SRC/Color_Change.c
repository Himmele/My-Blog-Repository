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

PgColor_t Colors[5] = {Pg_BLACK, Pg_YELLOW, Pg_MAGENTA, Pg_CYAN, Pg_DGREEN};
int base_Color = -1;
extern int WindowCounter;
extern PtWidget_t* Windows[5];

int
Color_Change( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	int i, previous;
	PtWidget_t* thisWindow;
	
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	thisWindow = ApGetInstance( widget );
	
	if(ApName(widget) == ABN_ButtonRed)
	{
		PtSetResource(ApGetWidgetPtr(thisWindow, ABN_ColorRect), Pt_ARG_FILL_COLOR, Pg_RED, 0);
	}
	else if(ApName(widget) == ABN_ButtonGreen)
	{
		PtSetResource(ApGetWidgetPtr(thisWindow, ABN_ColorRect), Pt_ARG_FILL_COLOR, Pg_GREEN, 0);
	}
	else if(ApName(widget) == ABN_ButtonBlue)
	{
		PtSetResource(ApGetWidgetPtr(thisWindow, ABN_ColorRect), Pt_ARG_FILL_COLOR, Pg_BLUE, 0);
	}
	else if(ApName(widget) == ABN_ButtonPrevious)
	{
		previous = -1;
		for(i = 0; i < WindowCounter; i++)
		{
			if(Windows[i] == thisWindow)
			{
				previous = i - 1;
				break;
			}
		}
		
		if(previous != -1 && Windows[previous])
		{
			base_Color++;
			if(base_Color >= 5)
			{
				base_Color = 0;
			}
			
			//PtSetResource(Windows[previous], Pt_ARG_FILL_COLOR, Colors[base_Color], 0);
			PtSetResource(ApGetWidgetPtr(Windows[previous], ABN_ColorRect), Pt_ARG_FILL_COLOR, Colors[base_Color], 0);
		}	
	}	

	return( Pt_CONTINUE );

	}

