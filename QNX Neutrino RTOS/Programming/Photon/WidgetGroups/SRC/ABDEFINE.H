/* Define header for application - AppBuilder 2.01  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_PtBigButton;
#define ABW_PtBigButton                      AbGetABW( ABN_PtBigButton )
extern const int ABN_PtToggleGroup;
#define ABW_PtToggleGroup                    AbGetABW( ABN_PtToggleGroup )
extern const int ABN_PtToggleButton1;
#define ABW_PtToggleButton1                  AbGetABW( ABN_PtToggleButton1 )
extern const int ABN_PtToggleButton2;
#define ABW_PtToggleButton2                  AbGetABW( ABN_PtToggleButton2 )
extern const int ABN_PtToggleButton3;
#define ABW_PtToggleButton3                  AbGetABW( ABN_PtToggleButton3 )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

#define AB_OPTIONS "s:x:y:h:w:S:"
