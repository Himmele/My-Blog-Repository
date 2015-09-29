/* Import (extern) header for application - AppBuilder 2.01  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApDialogLink_t AboutDlg;
extern ApWindowLink_t NewWindow;
extern ApWidget_t AbWidgets[ 12 ];

extern ApMenuLink_t FileMenu;
extern ApMenuLink_t HelpMenu;

#ifdef __cplusplus
extern "C" {
#endif
int Quit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int AboutDlg_Setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int NewWindow_Close( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int NewWindow_Setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Color_Change( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Color_Change( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
