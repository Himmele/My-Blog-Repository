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

/* Application Options string */
const char ApOptions[] =
	AB_OPTIONS "a:b:c:pqr"; /* Add your options in the "" */


int
init( int argc, char *argv[] )

	{
	int option;
	
	/* eliminate 'unreferenced' warnings */
	argc = argc, argv = argv;
	   
    while (( option = getopt(argc, argv, ApOptions)) != -1 )
        switch ( option )
        {
            case 'a' : 
            case 'b' : 
            case '?' : PtMessageBox(NULL, "Initialization", "starting", NULL, NULL);
            // default : PtMessageBox(NULL, "default", "default", NULL, NULL);
        }

	return( Pt_CONTINUE );

	}

