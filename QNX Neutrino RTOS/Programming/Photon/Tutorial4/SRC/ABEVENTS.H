/* Event header for application - AppBuilder 2.01  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 5, 0, 0L, 0L, 0L, &FileMenu, NULL, "base_File", 2007, NULL, 0, 1, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &HelpMenu, NULL, "base_Help", 2007, NULL, 0, 1, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_AboutDlg[] = {
	{ 9, 0, 0L, 0L, 0L, NULL, NULL, "AboutDone", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_FileMenu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "File_Quit", 2009, Quit, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_HelpMenu[] = {
	{ 4, 0, 0L, 0L, 0L, &AboutDlg, NULL, "Help_About", 2009, AboutDlg_Setup, 0, 9, 0, 0, },
	{ 0 }
	};

const char ApOptions[] = AB_OPTIONS;
