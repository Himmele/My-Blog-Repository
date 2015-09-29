/* Define header for application - AppBuilder 2.01  */

#if defined(__cplusplus)
extern "C" {
#endif

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_SearchButton;
#define ABW_SearchButton                     AbGetABW( ABN_SearchButton )
extern const int ABN_PtMyButton;
#define ABW_PtMyButton                       AbGetABW( ABN_PtMyButton )
extern const int ABN_PlatformButton;
#define ABW_PlatformButton                   AbGetABW( ABN_PlatformButton )

/* 'Search' Window link */
extern const int ABN_Search;
#define ABW_Search                           AbGetABW( ABN_Search )
extern const int ABN_TextString;
#define ABW_TextString                       AbGetABW( ABN_TextString )
extern const int ABN_SearchNow;
#define ABW_SearchNow                        AbGetABW( ABN_SearchNow )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"

#if defined(__cplusplus)
}
#endif

