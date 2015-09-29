#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <devctl.h>

typedef union _my_devctl_msg {
        int tx;             //Filled by client on send
        int rx;             //Filled by server on reply
} data_t;

#define MY_CMD_CODE      1
#define MY_DEVCTL_GETVAL __DIOF(_DCMD_MISC,  MY_CMD_CODE + 0, int)
#define MY_DEVCTL_SETVAL __DIOT(_DCMD_MISC,  MY_CMD_CODE + 1, int)
#define MY_DEVCTL_SETGET __DIOTF(_DCMD_MISC, MY_CMD_CODE + 2, union _my_devctl_msg)

int main(int argc, char *argv[]) 
{
	char buffer[16];
	int fd = open("/dev/sample", O_RDWR);
	read(fd, buffer, 16);
	printf("%s\n", buffer);
	if(argc < 2)
	{
		write(fd, "Hello World", strlen("Hello World"));
	}
	else
	{
		write(fd, argv[1], strlen(argv[1]));
	}
	read(fd, buffer, 16);
	printf("%s\n", buffer);
	
	int ret, val;
    data_t  data;
    /* Find out what the value is set to initially */
    val = -1;
    ret = devctl(fd, MY_DEVCTL_GETVAL, &val, sizeof(val), NULL);
    printf("GET returned %d w/ server value %d \n", ret, val);

    /* Set the value to something else */
    val = 25;
    ret = devctl(fd, MY_DEVCTL_SETVAL, &val, sizeof(val), NULL);
    printf("SET returned %d \n", ret);

    /* Verify we actually did set the value */
    val = -1;
    ret = devctl(fd, MY_DEVCTL_GETVAL, &val, sizeof(val), NULL);
    printf("GET returned %d w/ server value %d == 25? \n", ret, val);

    /* Now do a set/get combination */
    memset(&data, 0, sizeof(data));
    data.tx = 50;
    ret = devctl(fd, MY_DEVCTL_SETGET, &data, sizeof(data), NULL);
    printf("SETGET returned with %d w/ server value %d == 25?\n", ret, data.rx);

    /* Check set/get worked */
    val = -1;
    ret = devctl(fd, MY_DEVCTL_GETVAL, &val, sizeof(val), NULL);
    printf("GET returned %d w/ server value %d == 50? \n", ret, val);

	return EXIT_SUCCESS;
}
