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
Intersection( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	PhArea_t AreaRect1, AreaRect2;
	PhRect_t Rect1, Rect2;
		
	PhPoint_t* PosRect1;
	PhPoint_t* PosRect2;
	PhDim_t* DimRect1;
	PhDim_t* DimRect2;
	
	PtGetResource(ABW_PtRect1, Pt_ARG_POS, &PosRect1, 0);
	PtGetResource(ABW_PtRect1, Pt_ARG_DIM, &DimRect1, 0);
	PtGetResource(ABW_PtRect2, Pt_ARG_POS, &PosRect2, 0);
	PtGetResource(ABW_PtRect2, Pt_ARG_DIM, &DimRect2, 0);
	
	printf("pos of rect1: %d:%d\n", PosRect1->x, PosRect1->y);
	printf("pos of rect2: %d:%d\n", PosRect2->x, PosRect2->y);
	printf("size of rect1: %d:%d\n", DimRect1->w, DimRect1->h);
	printf("size of rect2: %d:%d\n", DimRect2->w, DimRect2->h);
	
	AreaRect1.pos = *PosRect1;
	AreaRect2.pos = *PosRect2;
	AreaRect1.size = *DimRect1;
	AreaRect2.size = *DimRect2;
	PhAreaToRect(&AreaRect1, &Rect1);
	PhAreaToRect(&AreaRect2, &Rect2);
	
	if(PhRectIntersect(&Rect1, &Rect2) == 0)
		printf("no intersection\n");
	
	PhRectToArea(&Rect1, &AreaRect1);
	
	PosRect1->x = AreaRect1.pos.x;
	PosRect1->y = AreaRect1.pos.y;
	DimRect1->w = AreaRect1.size.w;
	DimRect1->h = AreaRect1.size.h;
	
	printf("pos of rect1: %d:%d\n", PosRect1->x, PosRect1->y);
	printf("pos of rect2: %d:%d\n", PosRect2->x, PosRect2->y);
	printf("size of rect1: %d:%d\n", DimRect1->w, DimRect1->h);
	printf("size of rect2: %d:%d\n", DimRect2->w, DimRect2->h);
	
	PtSetResource(ABW_PtRect1, Pt_ARG_FILL_COLOR, Pg_BLUE, 0);

	return( Pt_CONTINUE );

	}


int
Timer( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	PtMessageBox(NULL, "Timer", "timer event happened!", NULL, NULL);

	return( Pt_CONTINUE );

	}

