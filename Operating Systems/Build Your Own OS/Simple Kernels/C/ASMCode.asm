; NASM ASMCode.asm -f elf
; produces a elf 32 bit binary

global DisableInterrupts
global LeaveKernel

DisableInterrupts:	
			cli
			ret
			
LeaveKernel:
			mov esp,ebp
			pop ebp
			retf
