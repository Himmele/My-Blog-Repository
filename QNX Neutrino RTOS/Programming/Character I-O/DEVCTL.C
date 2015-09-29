/* For "devctl()" */
#include <devctl.h>
#include <sys/dcmd_chr.h>

/* For "open()" */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* For Errors */
#include <stdlib.h>
#include <stdio.h>


int main(void)
{
    int data, fd, toggle = 1;

    /* Open the device we wish to manipulate. */
    if((fd = open ("/dev/kbd", O_RDONLY)) == -1)
    {
     	fprintf(stderr, "Error with open() on /dev/kbd.  Make sure exists.\n");
     	perror (NULL);
     	exit(EXIT_FAILURE);
    }

    while(1)
    {
    		switch(toggle)
    		{
        		case 1:
        		{
          		/*
           		Lets now turn on Num Lock and make sure that 
           		Caps and Scroll lock are turned off.
          		*/
          		data = (_CONCTL_NUM_CHG | _CONCTL_NUM) | _CONCTL_CAPS_CHG | _CONCTL_SCROLL_CHG;
          		break;
        		}
        		case 2:
        		{
          		/*
           		Turn off Num Lock and now turn on Caps Lock 
           		(Scroll lock is already off).
          		*/
          		data = _CONCTL_NUM_CHG | (_CONCTL_CAPS_CHG | _CONCTL_CAPS);
          		break;
        		}
        		case 3:
        		{
          		/*
           		Turn off Caps lock and turn on Scroll lock 
           		(Num lock is already off).
          		*/
          		data = _CONCTL_CAPS_CHG | (_CONCTL_SCROLL_CHG | _CONCTL_SCROLL);
          		toggle = 0;
          		break;
        		}
    		}

    		/* Explanation below. */
    		if (devctl (fd, DCMD_CHR_SERCTL, &data, sizeof(data), NULL))
    		{
        		fprintf(stderr, "Error setting KBD.\n");
        		perror (NULL);
        		exit(EXIT_FAILURE);
    		}

    		sleep(1);
    		toggle++;
    }

    return (1);
}