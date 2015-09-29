#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main( void )
  {
    int fd;
    struct termios termios_p;
    speed_t speed;

    fd = open("/dev/ser1", O_RDWR);
    tcgetattr(fd, &termios_p);

    /*
     *  get input and output baud rate
     */
    speed = cfgetospeed(&termios_p);
    printf("output baud rate: %ld\n", speed );
    speed = cfgetispeed(&termios_p);
    printf("input baud rate: %ld\n", speed);

    close( fd );
    return EXIT_SUCCESS;
  }