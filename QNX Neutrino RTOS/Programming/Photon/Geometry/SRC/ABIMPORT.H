/* Import (extern) header for application - AppBuilder 2.01  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 5 ];


#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int Intersection( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int Timer( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#endif
