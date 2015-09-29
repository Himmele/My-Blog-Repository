/* Link header for application - AppBuilder 2.01  */

#if defined(__cplusplus)
extern "C" {
#endif

extern ApContext_t AbContext;

ApWindowLink_t base = {
	"base.wgtw",
	&AbContext,
	AbLinks_base, 0, 3
	};

ApWindowLink_t Search = {
	"Search.wgtw",
	&AbContext,
	AbLinks_Search, 4, 1
	};


#if defined(__cplusplus)
}
#endif

