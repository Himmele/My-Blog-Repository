#include <stdio.h>
#include <stdlib.h>
#include <Pt.h>
#define EVENT_SIZE sizeof( PhEvent_t ) + 1000

// compile using: qcc -o hello hello.c -lph -Bstatic -lph -Bdynamic -lphrender

int main( int argc, char *argv[] )
{
	PtArg_t args[3];
    int n;
    PtWidget_t *window;
    int push_button_cb( PtWidget_t *, void *, PtCallbackInfo_t *); 
    PtCallback_t callbacks[] = {{push_button_cb, NULL}};
    PhEvent_t *event;

    if (PtInit(NULL) == -1)				// initialize the widget library
       PtExit(EXIT_FAILURE);

    window = PtCreateWidget(PtWindow, Pt_NO_PARENT, 0, NULL);		// create the parent widget (main window)

    n = 0;
    PtSetArg(&args[n++], Pt_ARG_TEXT_STRING, "Press to exit", 0); 

    PtSetArg(&args[n++], Pt_CB_ACTIVATE, callbacks, sizeof(callbacks)/sizeof(callbacks[0])); 
    PtCreateWidget(PtButton, window, n, args);								// create a push button and associate it with callback "PushButton"
    //PtAddCallback(PtCreateWidget(PtButton, window, 0, NULL), Pt_CB_ACTIVATE, push_button_cb, NULL);	// associate with callback

    PtRealizeWidget(window); 	//make a widget and its children visible and possibly interactive (DO NOT FORGET THESE CALL)
    
    // PtMainLoop start
    
    if( NULL == ( event = malloc( EVENT_SIZE ) ) )
          exit( EXIT_FAILURE );
     
    while( 1 ) 
    {
          switch( PhEventNext( event, EVENT_SIZE ) )	// provide synchronous event notification
          {
              case Ph_EVENT_MSG:
                 PtEventHandler( event );		// determine the widgets involved in an event and invoke the appropriate callback function
                 break;
              case -1:
                 perror( "PhEventNext failed" );
                 break;
     	  }
   }
    
    // PtMainLoop end

    //PtMainLoop(); 	// implement an application main loop (same as above)
    return (EXIT_SUCCESS);
}

int push_button_cb(PtWidget_t *w, void *data, PtCallbackInfo_t *cbinfo) 
{ 
    printf( "I was pushed\n" ); 
    PtExit( EXIT_SUCCESS ); 

    /* This line won't be reached, but it keeps the compiler happy. */

    return( Pt_CONTINUE );
}