/* Y o u r   D e s c r i p t i o n                       */
/*                            AppBuilder Photon Code Lib */
/*                                         Version 2.01  */

/* Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <new.h>

/* Local headers */
#include "ablibs.h"
#include "abimport.h"
#include "proto.h"

class QNX
{
	char* platform;
	
	public:
		QNX()
		{
			platform = new char[32];
			strcpy(platform, "x86");
		}
		
		~QNX()
		{
			delete [] platform;
		}
		
		char* GetPlatform()
		{
			return platform;
		}		
};
		

int
MyButton( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	char* string = new char[32];
	strcpy(string, "C++");
	
	PtMessageBox(NULL, string, string, NULL, NULL);

	return( Pt_CONTINUE );

	}

int
GetQNXPlatform( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )

	{

	/* eliminate 'unreferenced' warnings */
	widget = widget, apinfo = apinfo, cbinfo = cbinfo;
	
	QNX qnx;	
	PtMessageBox(NULL, "QNX", qnx.GetPlatform(), NULL, NULL);
	
	return( Pt_CONTINUE );

	}