/* Link header for application - AppBuilder 2.01  */

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 2
	};

static ApItem_t ApItems_FileMenu[ 6 ] = {
	{ 1, 1, 0, NULL, 0, "File_New", "New", NULL },
	{ 1, 1, 0, NULL, 0, "File_Save", "Save", NULL },
	{ 1, 1, 0, NULL, 0, "File_Save_As", "Save As", NULL },
	{ 1, 16, 0, NULL, 4, "", "", NULL },
	{ 1, 1, 0, NULL, 0, "File_Quit", "Quit", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t FileMenu = {
	"FileMenu",
	"",
	NULL,
	NULL,
	-2,
	ApItems_FileMenu,
	& AbContext,
	AbLinks_FileMenu,
	3, 1, 5
	};

static ApItem_t ApItems_HelpMenu[ 2 ] = {
	{ 1, 1, 0, NULL, 0, "Help_About", "About", NULL },
	{ 0, 0, NULL, NULL, 0, NULL, NULL, NULL } };

ApMenuLink_t HelpMenu = {
	"HelpMenu",
	"",
	NULL,
	NULL,
	-2,
	ApItems_HelpMenu,
	& AbContext,
	NULL,
	9, 0, 1
	};

