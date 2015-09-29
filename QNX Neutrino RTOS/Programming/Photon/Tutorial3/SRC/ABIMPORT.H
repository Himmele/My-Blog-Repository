/* Import (extern) header for application - AppBuilder 2.01  */

#include "abdefine.h"

extern ApWindowLink_t base;
extern ApWidget_t AbWidgets[ 3 ];

extern ApMenuLink_t FileMenu;
extern ApMenuLink_t HelpMenu;

#ifdef __cplusplus
extern "C" {
#endif
int Quit( PtWidget_t *widget, ApInfo_t *data, PtCallbackInfo_t *cbinfo );
#ifdef __cplusplus
}
#endif
