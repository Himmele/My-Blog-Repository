#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
        char Kernel[8*1024] = {0};
        int Floppy, File;
        
        File = open("./Kernel32.bin", O_RDONLY);
        read(File, Kernel, 8*1024);
        close(File);

        Floppy = open("/dev/fd0", O_RDWR);
        lseek(Floppy, 512, SEEK_CUR);
        write(Floppy, Kernel, 8*1024);
        close(Floppy);
}