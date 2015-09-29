[BITS 16]
[ORG 0x100]

	SECTION .text

		mov ah,0x4a	; resize used memory
		mov bx,0x1000	; # of paragraphs
		int 0x21	; es is still unmodified

		mov ah,0x48	; allocate a 64k block
		mov bx,0x1000	; # of paragraphs
		int 0x21
		mov [buffer],ax	; seg address of new block

		mov ax,0x3d00	; open file, read only
		mov dx,filename
		int 0x21
		jc near file_error
		mov [file_handle],ax


; PMODE INITALIZATION STATS HERE

;  ENABLE A20 LINE AND DISABLE INTS

A20:		cli
		call .discard_out
		call .command_wait
		mov al,0xd1
		out 0x64,al
		call .command_wait
		mov al,0xdf
		out 0x60,al
		call .command_wait
		jmp .done

.command_wait:	in al,0x64
		test al,0x02
		jnz .command_wait
		ret
.discard_out:	in al,0x64
		test al,0x01
		jnz .read
		ret
.read:		in al,0x60
		jmp .discard_out


.done:



		mov ax,cs	; save some values to return to real mode
		mov [Real_CS],ax
		mov [Real_CS2],ax
		lea ax,[Goto_Real]
		mov [Real_IP],ax
		lea ax,[Goto_Real2]
		mov [Real_IP2],ax
		
		xor eax,eax	; set base address in selector table
		mov ax,cs		
		shl eax,4
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

;***read a 64k section to buffer

		mov ah,0x3f
		mov bx,[file_handle]
		mov cx,0xffff
		mov dx,[buffer]
		mov ds,dx
		xor dx,dx
		int 0x21
		mov bx,cs
		mov ds,bx
		jc  near file_error
		mov [bytes_read],ax

		cli
		mov al,0x80	; disable nmi
		out 0x70,al

		mov eax,cr0	; SET PMODE BIT IN CR0
		or al,1
		mov cr0,eax

		jmp SYS_CODE_SEL:Goto_PMode

Goto_PMode:
[BITS 32]

		mov ax,SYS_DATA_SEL
		mov ds,ax


;***copy from buffer to extended memory
; TRANSFER STUFF IN [buffer]:0 TO FOUR_MEG_DATA_SEL:[TARGET]

		xor ecx,ecx
                mov cx,[bytes_read]
		cld
		xor esi,esi
		mov si,[buffer]
		shl esi,4
                mov edi,[Target]
                mov ax,FOUR_MEG_DATA_SEL
                mov ds,ax
                mov es,ax

		rep movsb

		mov ax,SYS_DATA_SEL
		mov ds,ax
                mov [Target],edi

		jmp REAL_CODE_SEL:Goto_16bit_Pmode

Goto_16bit_Pmode:
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
		mov ds,ax
		mov es,ax

		mov al,0x00	; enable nmi
		out 0x70,al
		sti

		mov ax,[bytes_read]
		cmp ax,0xffff
		je near File_Move

		mov ah,0x3e	; close file
		mov bx,[file_handle]
		int 0x21


;***CALL KERNEL FILE***

		cli
		mov al,0x80	; disable nmi
		out 0x70,al

		mov eax,cr0	; SET PMODE BIT IN CR0
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
		mov esp,0x3fffff
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
		mov esp,0xffff

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

		mov al,0x00	; enable nmi
		out 0x70,al
		sti
		jmp exit

file_error:	mov ah,0x3e	; close file
		mov bx,[file_handle]
		int 0x21

		mov dx,error_msg
		mov ah,0x09
		int 0x21
		jmp exit


exit:		mov ah,0x49	; free memory block
		mov dx,[buffer]
		mov es,dx
		int 0x21

		mov ah,0x4c
		int 0x21


	SECTION .data

buffer:		dw 0
file_handle:	dw 0
error_msg:	db 'Error: could not open ',0x22,'kernel32.bin',0x22,'$'
filename:	db 'KERNEL32.BIN',0x00
bytes_read:	dw 0

Real_IP:	dw 0
Real_CS:	dw 0
Real_IP2:	dw 0
Real_CS2:	dw 0
Target:         dd 0x100000


GDTR:
.Size:		dw GDT_END 
.Address:	dd 0 

	

gdt0 equ $			; null entry
GDT:
.Entry0:	dw 0		; limit 15:0
		dw 0		; base 15:0
		db 0		; base 23:16
		db 0		; type
		db 0		; limit 19:16, flags
		db 0		; base 31:24


SYS_CODE_SEL equ $-gdt0		; code segment descriptor

.Entry1:	dw 0xFFFF
		dw 0x0		; base
		db 0x0         	; base
		db 0x9A         ; present, ring 0, code, non-conforming, readable
		db 0x40   	; 32 bit
		db 0


SYS_DATA_SEL equ $-gdt0		; data segment descriptor

.Entry2:	dw 0xFFFF
		dw 0x0    	; base
		db 0x0	  	; base
		db 0x92         ; present, ring 0, data, expand-up, writable
		db 0x40		; 32 bit
		db 0


FOUR_MEG_DATA_SEL equ $-gdt0	; 4meg data segment descriptor

.Entry3:	dw 0x03ff
		dw 0x0          ; base
		db 0x0          ; base
		db 0x92         ; present, ring 0, data, expand-up, writable
		db 0xc0		; 4k pages, 32 bit
		db 0


REAL_CODE_SEL equ $-gdt0	; code segment descriptor for 16 bit mode

.Entry4:	dw 0xFFFF
		dw 0x0   	; base
		db 0x0          ; base
		db 0x9A         ; present, ring 0, code, non-conforming, readable
		db 0x00   	; 16 bit
		db 0


REAL_DATA_SEL equ $-gdt0	; data segment descriptor for 16 bit mode

.Entry5:	dw 0xFFFF
		dw 0x0          ; base
		db 0x0          ; base
		db 0x92         ; present, ring 0, data, expand-up, writable
		db 0x00		; 16 bit
		db 0


KERNEL_SEL equ $-gdt0	; 4meg code segment descriptor

.Entry6:	dw 0x03ff
		dw 0x0          ; base
		db 0x0          ; base
		db 0x9A         ; present, ring 0, code, non-conforming, readable
		db 0xc0		; 4k pages, 32 bit
		db 0

GDT_END equ $-gdt0 -1


	SECTION .bss

