/* Define header for application - AppBuilder 2.01  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_MenuButton;
#define ABW_MenuButton                       AbGetABW( ABN_MenuButton )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

/* 'MyMenu' Menu link */
extern const int ABN_MyMenu;

#define AB_OPTIONS "s:x:y:h:w:S:"
