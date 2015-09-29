[BITS 32]

global _PrintASMMessage
global _LeaveKernel
global _IncrementValues

SECTION .text

_PrintASMMessage:	
			mov ebx,0xB8000
			mov esi,STRING							; pointer to STRING
			mov ecx,17								; number of characters
			mov edx,0								; index
REPEAT:
			mov ax,[esi+edx]
			and ax,0x00FF
			or ax,0x0700							; attribute byte
			mov [ebx+2*edx],ax
			inc edx
			loop REPEAT								; repeat ECX times
			ret

_LeaveKernel:
			mov esp,ebp
			pop ebp
			retf

_IncrementValues:
			push ebp									; save old stack frame                
			mov ebp,esp								; set up new stack frame             
			
			push ebx
			push ecx
			mov eax,[ebp+8]						; leftmost parameter
			mov ebx,[ebp+12]
			mov ecx,1
			add [eax],ecx
			add [ebx],ecx
			pop ecx
			pop ebx
			mov eax,0								; return value
			
			mov esp,ebp             			
			pop ebp									; restore old stack frame
			ret 8										; size of parameters on stack
			
SECTION .data

STRING:	
			db "Assembler in C++!"