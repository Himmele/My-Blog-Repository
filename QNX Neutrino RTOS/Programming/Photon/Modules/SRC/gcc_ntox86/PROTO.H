
/* ../Command.c */
int Command ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../Initialization.c */
int Initialization ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../Password.c */
int ValidatePassword ( void *data , char const *password_entered );
void *NewThread ( void *Data );
int Password ( PtWidget_t *widget , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../Setup.c */
int Setup ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../Setup_NewWindow.c */
int Setup_NewWindow ( PtWidget_t *link_instance , ApInfo_t *apinfo , PtCallbackInfo_t *cbinfo );

/* ../abmain.c */

/* ../init.c */
int init ( int argc , char *argv []);
