nasm ASMCode.asm -f coff
gpp -ffreestanding -nostdlib -nostdinc -fno-builtin -fno-rtti -fno-exceptions Kernel32.cpp -r -o Kernel32.o
ld -nostdlib -nostdinc -o Kernel32.bin -e _main --oformat binary -Ttext 0x00100000 Kernel32.o ASMCode.o

@ECHO OFF
PAUSE