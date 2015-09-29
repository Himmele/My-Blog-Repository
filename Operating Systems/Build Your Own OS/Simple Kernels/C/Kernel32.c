// gcc -c Kernel32.c --> produces a 32 bit binary object file
// ld Kernel32.o ASMCode.o -o Kernel32.bin -oformat binary -Ttext 0x7C00

// "-oformat binary" tells the linker you want your output file to be plain,
// no relocations, no header information, just a straight flat binary image

// "-Ttext 0x100000" tells the linker you want your "text" (code segment) 
// address to be at 1MB memory mark

extern DisableInterrupts();
extern LeaveKernel();

int main(void)
{
	char *str = "Hello, World!", *ch;
	unsigned short *vidmem = (unsigned short*) 0xb8000;
	unsigned i;
	
	// DisableInterrupts();
	
	for (ch = str, i = 0; *ch; ch++, i++)
		vidmem[i] = (unsigned char) *ch | 0x0700;

	LeaveKernel();		
}