#include <stdlib.h>
#include <termios.h>

int main( void )
{
    char Buffer[11];
    printf("RAW\n");
    
    raw( 0 );										// set standard input to RAW
    /*
     * standard input is now "raw"
     */
    
    read(0, Buffer, 7);							// read 7 characters from STDIN
    Buffer[7] = '\0';
    printf("raw string: %s\n", Buffer);
    
    unraw ( 0 );									// set standard input to UNRAW
    /*
     * standard input is now "unraw"
     */

    printf("\nUNRAW\n");
    read(0, Buffer, 7);							// read up to 7 charachters from the STDIN io-char input queue or until ENTER is pressed
    printf("unraw string: %s\n", Buffer);
    return EXIT_SUCCESS;
}

int raw( fd )
{
    struct termios termios_p;

    if(tcgetattr(fd, &termios_p))
      return( -1 );

    termios_p.c_cc[VMIN]  =  7;			// wait for 7 characters before returning from a read call
    termios_p.c_cc[VTIME] =  0;			// notify, if there is a pause time (in 1/10ths of a second)
    termios_p.c_cc[VLEFT] = 65;			// set LEFT to UP
    termios_p.c_cc[VUP] = 67;			// set UP to RIGHT
    termios_p.c_lflag &= ~( ECHO|ICANON|ISIG|ECHOE|ECHOK|ECHONL);
    termios_p.c_oflag &= ~( OPOST );
    return( tcsetattr( fd, TCSADRAIN, &termios_p ) );
}

int unraw( fd )
{
    struct termios termios_p;

    if( tcgetattr( fd, &termios_p ) )
      return( -1 );

    termios_p.c_lflag |= ( ECHO|ICANON|ISIG|ECHOE|ECHOK|ECHONL);
    termios_p.c_oflag |= ( OPOST );
    return( tcsetattr( fd, TCSADRAIN, &termios_p ) );
}