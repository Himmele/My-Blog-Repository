/* Import (extern) header for application - AppBuilder 2.01  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 6 ];


#ifdef __cplusplus
extern "C" {
#endif
int ToggleButton3( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int ToggleButton1( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Toggle( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int BigButton( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
