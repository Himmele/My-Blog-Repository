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
Command( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	//ApModifyItemState(&Menu, AB_ITEM_DIM, ABN_Submenu, NULL);
	ApModifyItemState(&Menu, AB_ITEM_SET, ABN_Toggle, ABN_ExclusiveToggle, NULL);
	
	ApModifyItemText(&Menu, ABN_Submenu, "&QNX");

	return( Pt_CONTINUE );

	}

