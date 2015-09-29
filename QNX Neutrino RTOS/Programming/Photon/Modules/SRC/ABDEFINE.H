/* Define header for application - AppBuilder 2.01  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_PtMyMenuButton;
#define ABW_PtMyMenuButton                   AbGetABW( ABN_PtMyMenuButton )
extern const int ABN_PtPasswordButton;
#define ABW_PtPasswordButton                 AbGetABW( ABN_PtPasswordButton )
extern const int ABN_PtHello_WorldButton;
#define ABW_PtHello_WorldButton              AbGetABW( ABN_PtHello_WorldButton )

/* 'new' Window link */
extern const int ABN_new;
#define ABW_new                              AbGetABW( ABN_new )
extern const int ABN_Plus;
#define ABW_Plus                             AbGetABW( ABN_Plus )

/* 'Hello_World' Window link */
extern const int ABN_Hello_World;
#define ABW_Hello_World                      AbGetABW( ABN_Hello_World )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

/* 'Menu' Menu link */
extern const int ABN_Menu;
extern const int ABN_Command;
extern const int ABN_Submenu;
extern const int ABN_Toggle;
extern const int ABN_ExclusiveToggle;
extern const int ABN_Quit;

#define AB_OPTIONS "s:x:y:h:w:S:"
