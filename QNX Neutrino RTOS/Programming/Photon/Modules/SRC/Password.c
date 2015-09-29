/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/neutrino.h>

/* Local headers */
#include "ablibs.h"
#include "globals.h"
#include "abimport.h"
#include "proto.h"


int ValidatePassword( void *data, char const *password_entered )
{
	if(!strcmp(password_entered, (char*)data))
		return Pt_PWD_ACCEPT;
	else
		return Pt_PWD_RETRY;
}

void* NewThread(void* Data)
{
	PtMessageBox(NULL, (char*) Data, "Thread", NULL, NULL);
	
	ThreadDestroy(0, 0, 0);
	return NULL;
}

int
Password( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{
	PhPoint_t location;
	char font[MAX_FONT_TAG];
	char String[32];
			
	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;

	PtSetResource(ApGetWidgetPtr(ApGetInstance(widget), ABN_PtPasswordButton), Pt_ARG_FILL_COLOR, Pg_BLUE, 0);
	PtGlobalFocusNext(widget, NULL);	// give focus to "Hello World" button
	
	location.x = 50;
	location.y = 50;
	
	strcpy(String, "Threads in QNX");
	ThreadCreate(NULL, NewThread, "Threads in QNX", NULL);
	
	PtPassword(ApGetInstance(widget), 
					   &location, 
					   NULL, 
					   NULL, 
					   "Password: ", 
					   PfGenerateFontName("Helvetica", PF_STYLE_ANTIALIAS, 12, font), 
					   NULL, 
					   NULL, 
					   PfGenerateFontName("Helvetica", PF_STYLE_ANTIALIAS, 12, font),
					   ValidatePassword, 
					   "QNX",
					   NULL,
					   Pt_CENTER | Pt_BLOCK_PARENT);

	return( Pt_CONTINUE );

	}

