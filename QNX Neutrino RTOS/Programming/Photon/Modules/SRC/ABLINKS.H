/* Link header for application - AppBuilder 2.01  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 4
	};

ApWindowLink_t new = {
	"new.wgtw",
	&AbContext,
	NULL, 4, 0
	};

ApWindowLink_t Hello_World = {
	"Hello_World.wgtw",
	&AbContext,
	NULL, 6, 0
	};

static ApItem_t ApItems_Menu[ 8 ] = {
	{ 1, 1, 4,"C", 0, "Command", "&Command", NULL },
	{ 1, 2, 0, NULL, 0, "Submenu", "Submenu", NULL },
	{ 2, 4, 0, NULL, 0, "Toggle", "Toggle", NULL },
	{ 2, 64, 0, NULL, 0, "ExclusiveToggle", "ExclusiveToggle", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 8, 0, NULL, 0, "", (char*) Initialization, NULL },
	{ 1, 1, 0, NULL, 0, "Quit", "Quit", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t Menu = {
	"Menu",
	"",
	NULL,
	NULL,
	-2,
	ApItems_Menu,
	& AbContext,
	AbLinks_Menu,
	7, 1, 7
	};

