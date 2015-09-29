/* Import (extern) header for application - AppBuilder 2.01  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWindowLink_t new;
extern ApWindowLink_t Hello_World;
extern ApWidget_t AbWidgets[ 7 ];

extern ApMenuLink_t Menu;

#ifdef __cplusplus
extern "C" {
#endif
int init( int argc, char **argv );
int Initialization( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Setup( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Command( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Setup_NewWindow( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Password( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Command( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
