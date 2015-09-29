/* Import (extern) header for application - AppBuilder 2.01  */

#if defined(__cplusplus)
extern "C" {
#endif

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWindowLink_t Search;
extern ApWidget_t AbWidgets[ 7 ];


#if defined(__cplusplus)
}
#endif


#ifdef __cplusplus
int MyButton( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
int GetQNXPlatform( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
extern "C" {
#endif
int init( int argc, char **argv );
int SearchIt( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
