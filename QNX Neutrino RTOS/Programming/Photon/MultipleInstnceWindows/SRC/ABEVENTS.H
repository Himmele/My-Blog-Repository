/* Event header for application - AppBuilder 2.01  */

#if defined(__cplusplus)
extern "C" {
#endif

static const ApEventLink_t AbApplLinks[] = {
	{ 3, 0, 0L, 0L, 0L, &base, NULL, NULL, 0, NULL, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_base[] = {
	{ 3, 0, 0L, 0L, 0L, &Search, NULL, "SearchButton", 2009, NULL, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PtMyButton", 2009, MyButton, 0, 0, 0, 0, },
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "PlatformButton", 2009, GetQNXPlatform, 0, 0, 0, 0, },
	{ 0 }
	};

static const ApEventLink_t AbLinks_Search[] = {
	{ 8, 0, 0L, 0L, 0L, NULL, NULL, "SearchNow", 2009, SearchIt, 0, 0, 0, 0, },
	{ 0 }
	};


#if defined(__cplusplus)
}
#endif

