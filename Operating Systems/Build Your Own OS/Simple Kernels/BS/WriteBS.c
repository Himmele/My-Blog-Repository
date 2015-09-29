/* write a boot sector to a floppy */

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
        char boot_buf[512] = {0};
        int floppy_desc, file_desc;
 
        
        file_desc = open("./BootSector", O_RDONLY);
        read(file_desc, boot_buf, 510);
        close(file_desc);
        
        boot_buf[510] = 0x55;
        boot_buf[511] = 0xAA;

        floppy_desc = open("/dev/fd0", O_RDWR);
        lseek(floppy_desc, 0, SEEK_CUR);
        write(floppy_desc, boot_buf, 512);
        close(floppy_desc);
}