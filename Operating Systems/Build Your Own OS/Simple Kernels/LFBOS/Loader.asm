; MS-DOS: 16 Bit Real-Mode
[BITS 16]

; .COM files expect to be loaded at offset 100h into their segment 
; execution then begins at 100h, i.e. right at the start of the program
[ORG 0x100]

SECTION .text

; Modify Memory Allocation
; function (AH): 4Ah
; entry parameters: ES:0 - address of block to modify allocation size
;                   BX - size of new block
; exit parameters: if the carry is set, then AX contains the error code 7, 8, or 9
;                  BX contains the maximum size possible (if error 8)
; this call is used to change the size of an allocated block. On entry, ES must 
; contain the segment address of the allocated block returned by the memory 
; allocation function. BX must contain the new size of this block in paragraphs. 
; While you can almost always reduce the size of a block, you cannot normally 
; increase the size of a block if other blocks have been allocated after the 
; block being modified. Keep this in mind when using this function.
		mov ah,0x4A	; resize used memory
		mov bx,0x1000 ; number of paragraphs
		int 0x21	; ES is still unmodified
		
; Allocate Memory
; function (AH): 48h
; entry parameters: BX - requested block size (in paragraphs)
; exit parameters: if no error (carry clear): AX:0 points at allocated memory block 
;                  if an error (carry set): BX - maximum possible allocation size
;                                           AX - error code (7 or 8)
; this call is used to allocate a block of memory. On entry into DOS, 
; BX contains the size of the requested block in paragraphs (groups of 16 bytes). 
; On exit, assuming no error, the AX register contains the segment address of the 
; start of the allocated block. If an error occurs, the block is not allocated and 
; the AX register is returned containing the error code. If the allocation request 
; failed due to insufficient memory, the BX register is returned containing the 
; maximum number of paragraphs actually available.
		mov ah,0x48	; allocate a 64k block
		mov bx,0x1000 ; number of paragraphs
		int 0x21
		mov [Buffer],ax ; segment address of new block

; Open File
; AH = 3Dh --> open existing file
; AL: access rights --> 0 - file opened for reading 
;                       1 - file opened for writing
;                       2 - file opened for reading and writing
; DS:DX - point at a zero terminated string containing the filename
; if the carry is set, AX contains one of the following error codes:
;                       2 - file not found 
;                       4 - too many open files
;                       5 - access denied 
;                       12 - invalid access
; if the carry is clear, AX contains the file handle value assigned by DOS
		mov ax,0x3D00
		mov dx,Filename
		int 0x21
		jc near File_Error
		mov [File_Handle],ax


; PROTECTED MODE INITALIZATION STATS HERE
; ENABLE A20 LINE AND DISABLE INTS

A20:	cli
		call .discard_out
		call .command_wait
		mov al,0xD1
		out 0x64,al
		call .command_wait
		mov al,0xDF
		out 0x60,al
		call .command_wait
		jmp .done

.command_wait:	in al,0x64
		test al,0x02
		jnz .command_wait
		ret
.discard_out: in al,0x64
		test al,0x01
		jnz .read
		ret
.read: in al,0x60
		jmp .discard_out
.done:


		mov ax,cs ; save some values to return to real mode
		mov [Real_CS],ax
		mov [Real_CS2],ax
		lea ax,[Goto_Real] ; load effictive address of label Goto_Real
		mov [Real_IP],ax
		lea ax,[Goto_Real2]
		mov [Real_IP2],ax
		
; all base addresses refer to real mode CS
		xor eax,eax	; set base address in selector table
		mov ax,cs		
		shl eax,4 ; build up physical address from current real mode CS
		mov [GDT.Entry1+2],ax
		mov [GDT.Entry2+2],ax
		mov [GDT.Entry4+2],ax
		mov [GDT.Entry5+2],ax
		shr eax,16
		mov [GDT.Entry1+4],al
		mov [GDT.Entry2+4],al
		mov [GDT.Entry4+4],al
		mov [GDT.Entry5+4],al

		xor eax,eax
		mov ax,cs		
		shl eax,4
		add eax,gdt0
		mov [GDTR.Address],eax
		lgdt [GDTR]


File_Move:
; Read From a File
; function (AH): 3Fh
; entry parameters: BX - file handle 
;                   CX - number of bytes to read
;                   DS:DX - array large enough to hold bytes read
; exit parameters: if the carry flag is set, AX contains one of the following error codes
;                  5 - access denied
;                  6 - invalid handle
;                  if the carry flag is clear, AX contains the number of bytes actually read from the file
; the read function is used to read some number of bytes from a file. The actual number of bytes 
; is specified by the CX register upon entry into DOS. The file handle, which specifies the file 
; from which the bytes are to be read, is passed in the BX register. The DS:DX register contains 
; the address of a buffer into which the bytes read from the file are to be stored. 
; On return, if there wasn't an error, the AX register contains the number of bytes actually read. 
; Unless the end of file (EOF) was reached, this number will match the value passed to DOS in the CX register. 
; If the end of file has been reached, the value return in AX will be somewhere between zero and the value 
; passed to DOS in the CX register. This is the only test for the EOF condition. 

; read a 64k section to buffer

		mov ah,0x3F
		mov bx,[File_Handle]
		mov cx,0xFFFF
		mov dx,[Buffer]
		mov ds,dx
		xor dx,dx
		int 0x21
		mov bx,cs
		mov ds,bx ; restore real mode data segment (.COM)
		jc  near File_Error
		mov [Bytes_Read],ax

		cli
		mov al,0x80	; disable NMI
		out 0x70,al

		mov eax,cr0	; SET PROTECTED MODE BIT IN CR0
		or al,1
		mov cr0,eax

		jmp SYS_CODE_SEL:Goto_PMode

Goto_PMode:
[BITS 32]

		mov ax,SYS_DATA_SEL
		mov ds,ax


; copy from buffer to extended memory
; TRANSFER STUFF IN [Buffer]:0 TO FOUR_MEG_DATA_SEL:[TARGET]

		xor ecx,ecx
      mov cx,[Bytes_Read]
		cld
		xor esi,esi
		mov si,[Buffer]
		shl esi,4 ; physical address of Buffer
      mov edi,[Target]
      mov ax,FOUR_MEG_DATA_SEL
      mov ds,ax
      mov es,ax
		rep movsb ; DS:SI --> ES:DI

		mov ax,SYS_DATA_SEL
		mov ds,ax
      mov [Target],edi

		jmp REAL_CODE_SEL:Goto_16Bit_Pmode

Goto_16Bit_Pmode:
[BITS 16]
		mov ax,REAL_DATA_SEL
		mov ds,ax
		mov es,ax

		mov eax,cr0
		and al,0xFE
		mov cr0,eax

		jmp far [Real_IP]
Goto_Real:
		mov ax,cs
		mov ds,ax ; .COM file
		mov es,ax

		mov al,0x00	; enable NMI
		out 0x70,al
		sti

		mov ax,[Bytes_Read]
		cmp ax,0xffff
		je near File_Move

		mov ah,0x3E	; close file
		mov bx,[File_Handle]
		int 0x21


;*** CALL KERNEL FILE ***

		cli
		mov al,0x80	; disable NMI
		out 0x70,al
		
		mov ax,0x4F02 ; switch to VESA mode 800x600x32
		mov bx,0x4115
		int 0x10

		mov eax,cr0	; SET PROTECTED MODE BIT IN CR0
		or al,1
		mov cr0,eax
		jmp SYS_CODE_SEL:Setup_Kernel

Setup_Kernel:
[BITS 32]
      mov ax,FOUR_MEG_DATA_SEL
      mov ds,ax
      mov es,ax
		mov gs,ax
		mov fs,ax
		mov ss,ax
		mov esp,0x3FFFFF ; set stack pointer at the end of the 4M segment
		call KERNEL_SEL:0x100000

		jmp REAL_CODE_SEL:Kernel_Done

Kernel_Done:
[BITS 16]
		mov ax,REAL_DATA_SEL
		mov ds,ax
		mov es,ax
		mov ss,ax
		mov gs,ax
		mov fs,ax
		mov esp,0xFFFF

		mov eax,cr0
		and al,0xFE
		mov cr0,eax

		jmp far [Real_IP2]
Goto_Real2:
		mov ax,cs
		mov ds,ax
		mov es,ax
		mov ss,ax
		mov gs,ax
		mov fs,ax

		mov al,0x00	; enable NMI
		out 0x70,al
		sti
		
		mov ax,0x3 ; switch back to text mode
		int 0x10
		
		jmp Exit

File_Error:	mov ah,0x3E	; close file
		mov bx,[File_Handle]
		int 0x21

		mov dx,Error_Msg
		mov ah,0x09
		int 0x21
		jmp Exit

Exit: mov ah,0x49	; free memory block
		mov dx,[Buffer]
		mov es,dx
		int 0x21

		mov ah,0x4C
		int 0x21


SECTION .data

Buffer:		dw 0
File_Handle:dw 0
Error_Msg:	db 'error: could not open ',0x22,'Kernel32.BIN',0x22,'$'
Filename:	db 'KERNEL32.BIN',0x00
Bytes_Read:	dw 0

Real_IP:		dw 0
Real_CS:		dw 0
Real_IP2:	dw 0
Real_CS2:	dw 0
Target:     dd 0x100000


GDTR:
.Size:		dw GDT_END 
.Address:	dd 0 

; $ is the HERE token --> gives the number of bytes since the start
gdt0 equ $
GDT:
.Entry0:	dw 0	; limit 15:0
		dw 0		; base 15:0
		db 0		; base 23:16
		db 0		; type
		db 0		; limit 19:16, flags
		db 0		; base 31:24


SYS_CODE_SEL equ $-gdt0		; code segment descriptor

.Entry1:	dw 0xFFFF
		dw 0x0		; base
		db 0x0      ; base
		db 0x9A     ; present, ring 0, code, non-conforming, readable
		db 0x40   	; 32 bit
		db 0


SYS_DATA_SEL equ $-gdt0		; data segment descriptor

.Entry2:	dw 0xFFFF
		dw 0x0    	; base
		db 0x0	  	; base
		db 0x92     ; present, ring 0, data, expand-up, writable
		db 0x40		; 32 bit
		db 0


FOUR_MEG_DATA_SEL equ $-gdt0	; 4M data segment descriptor

.Entry3:	dw 0xFFFF
		dw 0x0       ; base
		db 0x0       ; base
		db 0x92      ; present, ring 0, data, expand-up, writable
		db 0xCF		 ; 4k pages, 32 bit
		db 0


REAL_CODE_SEL equ $-gdt0	; code segment descriptor for 16 bit mode

.Entry4:	dw 0xFFFF
		dw 0x0   	; base
		db 0x0      ; base
		db 0x9A     ; present, ring 0, code, non-conforming, readable
		db 0x00   	; 16 bit
		db 0


REAL_DATA_SEL equ $-gdt0	; data segment descriptor for 16 bit mode

.Entry5:	dw 0xFFFF
		dw 0x0      ; base
		db 0x0      ; base
		db 0x92     ; present, ring 0, data, expand-up, writable
		db 0x00		; 16 bit
		db 0


KERNEL_SEL equ $-gdt0	; 4M code segment descriptor

.Entry6:	dw 0xFFFF
		dw 0x0      ; base
		db 0x0      ; base
		db 0x9A     ; present, ring 0, code, non-conforming, readable
		db 0xCF		; 4k pages, 32 bit
		db 0

GDT_END equ $-gdt0 -1


SECTION .bss

