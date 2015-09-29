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

/* Constants for the shapes in the menu */
#define RECTANGLE  1
#define CIRCLE     2
#define DOT        3
#define BLOB       4
#define POLYGON    5

int
ShapeMenuCB( PtWidget_t *widget, void *client_data, PtCallbackInfo_t *cbinfo )
{
  int shape_chosen = (int) client_data;

  widget=widget, client_data=client_data, cbinfo=cbinfo;

  /* This callback uses the client data to determine which shape was chosen. */

  switch (shape_chosen) 
  {
    case RECTANGLE: PtMessageBox(NULL, "Rectangle", "Rectangle", NULL, NULL);
                    				break;
    case CIRCLE:			PtMessageBox(NULL, "Circle", "Circle", NULL, NULL);
                   				break;
    case DOT:				PtMessageBox(NULL, "Dot", "Dot", NULL, NULL);
                    				break;
    case BLOB:			PtMessageBox(NULL, "Blob", "Blob", NULL, NULL);
                    				break;
    case POLYGON:		PtMessageBox(NULL, "Polygon", "Polygon", NULL, NULL);
                    				break;
    default:					printf ("Unknown shape: %d\n", shape_chosen);
  }

  return (Pt_CONTINUE);
}

int
InitMenu( PtWidget_t *widget, ApInfo_t *apinfo, PtCallbackInfo_t *cbinfo )
{
  PtArg_t args[3];
  PtWidget_t *menu, *new_item;

  /* eliminate 'unreferenced' warnings */
  widget = widget, apinfo = apinfo, cbinfo = cbinfo;
  //PtCreateWidget(PtWindow, Pt_DEFAULT_PARENT, 0, NULL);

  PtSetArg (&args[0], Pt_ARG_TEXT_STRING, "Rectangle", 0);
  PtSetArg (&args[1], Pt_ARG_ACCEL_KEY, "R", 0);
  new_item = PtCreateWidget( PtMenuButton, Pt_DEFAULT_PARENT, 2, args);
  PtAddCallback ( new_item, Pt_CB_ACTIVATE, ShapeMenuCB, (void *)RECTANGLE );

  PtSetArg (&args[0], Pt_ARG_TEXT_STRING, "Circle", 0);
  PtSetArg (&args[1], Pt_ARG_ACCEL_KEY, "C", 0);
  new_item = PtCreateWidget( PtMenuButton, Pt_DEFAULT_PARENT, 2, args);
  PtAddCallback ( new_item, Pt_CB_ACTIVATE, ShapeMenuCB, (void *)CIRCLE );

  /* Create a menu button for the submenu. */

  PtSetArg (&args[0], Pt_ARG_TEXT_STRING, "Miscellaneous", 0);
  PtSetArg (&args[1], Pt_ARG_ACCEL_KEY, "M", 0);
  PtSetArg (&args[2], Pt_ARG_BUTTON_TYPE, Pt_MENU_RIGHT, 0 );
  new_item = PtCreateWidget( PtMenuButton, Pt_DEFAULT_PARENT, 3, args);

  /* Save the current default parent. */

  menu = PtGetParentWidget();

  /* Create a submenu. It becomes the new default parent. */

  PtSetArg (&args[0], Pt_ARG_MENU_FLAGS, Pt_MENU_CHILD, Pt_MENU_CHILD); 
  new_item = PtCreateWidget( PtMenu, Pt_DEFAULT_PARENT, 1, args);

  /* Add items to the submenu. */

  PtSetArg (&args[0], Pt_ARG_TEXT_STRING, "Dot", 0);
  PtSetArg (&args[1], Pt_ARG_ACCEL_KEY, "D", 0);
  new_item = PtCreateWidget( PtMenuButton, Pt_DEFAULT_PARENT, 2, args);
  PtAddCallback ( new_item, Pt_CB_ACTIVATE, ShapeMenuCB, (void *)DOT );

  PtSetArg (&args[0], Pt_ARG_TEXT_STRING, "Blob", 0);
  PtSetArg (&args[1], Pt_ARG_ACCEL_KEY, "B", 0);
  new_item = PtCreateWidget( PtMenuButton, Pt_DEFAULT_PARENT, 2, args);
  PtAddCallback ( new_item, Pt_CB_ACTIVATE, ShapeMenuCB, (void *)BLOB);

  /* Restore the current default parent. */

  PtSetParentWidget (menu);

  /* Continue adding items to the top menu. */

  PtSetArg (&args[0], Pt_ARG_TEXT_STRING, "Polygon", 0);
  PtSetArg (&args[1], Pt_ARG_ACCEL_KEY, "P", 0);
  new_item = PtCreateWidget( PtMenuButton, Pt_DEFAULT_PARENT, 2, args);
  PtAddCallback ( new_item, Pt_CB_ACTIVATE, ShapeMenuCB, (void *)POLYGON);

  return( Pt_CONTINUE );

}

