/* Define header for application - AppBuilder 2.01  */

#if defined(__cplusplus)
extern "C" {
#endif

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_PtRect1;
#define ABW_PtRect1                          AbGetABW( ABN_PtRect1 )
extern const int ABN_PtRect2;
#define ABW_PtRect2                          AbGetABW( ABN_PtRect2 )
extern const int ABN_IntersectionButton;
#define ABW_IntersectionButton               AbGetABW( ABN_IntersectionButton )
extern const int ABN_PtMyTimer;
#define ABW_PtMyTimer                        AbGetABW( ABN_PtMyTimer )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"

#if defined(__cplusplus)
}
#endif

