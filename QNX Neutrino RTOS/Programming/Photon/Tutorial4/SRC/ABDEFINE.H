/* Define header for application - AppBuilder 2.01  */

/* 'base' Window link */
extern const int ABN_base;
#define ABW_base                             AbGetABW( ABN_base )
extern const int ABN_base_File;
#define ABW_base_File                        AbGetABW( ABN_base_File )
extern const int ABN_base_Help;
#define ABW_base_Help                        AbGetABW( ABN_base_Help )

/* 'AboutDlg' Dialog link */
extern const int ABN_AboutDlg;
#define ABW_AboutDlg                         AbGetABW( ABN_AboutDlg )
extern const int ABN_About_Version;
#define ABW_About_Version                    AbGetABW( ABN_About_Version )
extern const int ABN_AboutDone;
#define ABW_AboutDone                        AbGetABW( ABN_AboutDone )

#define AbGetABW( n ) ( AbWidgets[ n ].wgt )

/* 'FileMenu' Menu link */
extern const int ABN_FileMenu;
extern const int ABN_File_New;
extern const int ABN_File_Save;
extern const int ABN_File_Save_As;
extern const int ABN_File_Quit;

/* 'HelpMenu' Menu link */
extern const int ABN_HelpMenu;
extern const int ABN_Help_About;

#define AB_OPTIONS "s:x:y:h:w:S:"
