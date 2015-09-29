/* Event header for application - AppBuilder 2.01  */

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 5, 0, 0L, 0L, 0L, &MyMenu, NULL, "MenuButton", 2009, NULL, 0, 1, 0, 0, },
	{ 0 }
	};

const char ApOptions[] = AB_OPTIONS;
