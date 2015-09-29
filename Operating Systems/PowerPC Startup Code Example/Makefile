PREFIX   = powerpc-eabi-
ASM      = $(PREFIX)as
CC       = $(PREFIX)gcc
CPP      = $(PREFIX)g++
LD       = $(PREFIX)g++
OBJCOPY  = $(PREFIX)objcopy
OBJDUMP  = $(PREFIX)objdump

CFLAGS   = -c -g -nostdlib -fno-threadsafe-statics -fno-exceptions -fno-rtti -meabi -mcpu=405
LDFLAGS  = -Wl,-Tlinker.ld -nostdlib -fno-exceptions -fno-rtti -meabi -mcpu=405

all: 
	$(ASM) -g startup.S -o startup.o
	$(CPP) $(CFLAGS) main.cpp
	$(LD)  $(LDFLAGS) main.o startup.o -o ppc.elf
	$(OBJCOPY) ppc.elf ppc.bin --output-target=binary
	$(OBJDUMP) -h ppc.elf
	
clean:
	rm -f *.o
	rm -f *.elf
	rm -f *.bin
	
rebuild: clean all
