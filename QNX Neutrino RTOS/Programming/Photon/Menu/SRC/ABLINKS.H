/* Link header for application - AppBuilder 2.01  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 1
	};

static ApItem_t ApItems_MyMenu[ 2 ] = {
	{ 1, 8, 0, NULL, 0, "", (char*) InitMenu, NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t MyMenu = {
	"MyMenu",
	"",
	NULL,
	NULL,
	-2,
	ApItems_MyMenu,
	& AbContext,
	NULL,
	2, 0, 1
	};

