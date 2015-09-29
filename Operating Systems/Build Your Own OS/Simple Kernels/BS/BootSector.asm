[BITS 16]

[ORG 0x0]

SECTION .text

jmp 07C0h:Start

; PROTECTED MODE INITALIZATION STATS HERE
; ENABLE A20 LINE AND DISABLE INTS

Start:mov ax,cs
      mov ds,ax
      mov es,ax
          
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
Reset:
		mov ax, 0
      mov dl, 0 ; Drive=0 (=A)
      int 13h
      jc Reset ; ERROR --> reset again

Read:
      mov ax,[Buffer]    ; ES:BX = 1000:0000
      mov es,ax
      mov bx,0
      mov ah, 2          ; load disk data to ES:BX
      mov al, 16         ; load 16 sectors
      mov ch, 0          ; cylinder=0
      mov cl, 2          ; sector=2
      mov dh,0           ; head=0
      mov dl,0           ; drive=0
      int 13h            ; read!

      jc Read            ; ERROR --> Try again
     	mov ax,cs          
		mov es,ax

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

;*** CALL KERNEL FILE ***

		cli
		mov al,0x80	; disable NMI
		out 0x70,al

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
		
SECTION .data

Buffer:		dw 0x1000
Bytes_Read:	dw 0x2000

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

.Entry3:	dw 0x03ff
		dw 0x0       ; base
		db 0x0       ; base
		db 0x92      ; present, ring 0, data, expand-up, writable
		db 0xc0		 ; 4k pages, 32 bit
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

.Entry6:	dw 0x03ff
		dw 0x0      ; base
		db 0x0      ; base
		db 0x9A     ; present, ring 0, code, non-conforming, readable
		db 0xc0		; 4k pages, 32 bit
		db 0

GDT_END equ $-gdt0 -1


SECTION .bss
