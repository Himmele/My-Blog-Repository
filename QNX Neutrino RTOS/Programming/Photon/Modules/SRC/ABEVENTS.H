/* Event header for application - AppBuilder 2.01  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, Setup, 0, 0, 0, 0, },
	{ 3, 0, 0L, 0L, 0L, &new, NULL, NULL, 0, Setup_NewWindow, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 8, 2, 99L, 4L, 0L, NULL, NULL, "base", 1010, Command, 0, 0, 0, 0, },
	{ 5, 0, 0L, 0L, 0L, &Menu, NULL, "PtMyMenuButton", 2009, NULL, 0, 1, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtPasswordButton", 2009, Password, 0, 0, 0, 0, },
	{ 3, 0, 0L, 0L, 0L, &Hello_World, NULL, "PtHello_WorldButton", 2009, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_Menu[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "Command", 2009, Command, 0, 0, 0, 0, },
	{ 0 }
	};

