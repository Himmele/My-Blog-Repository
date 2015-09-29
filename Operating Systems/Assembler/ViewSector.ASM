;read a disk sector form a drive

INCLUDE MACRO.BIB

.MODEL SMALL
.STACK 256


.CODE
			
START:			mov ax,@DATA
			mov ds,ax
			mov di,65000d		; offset of buffer to store sector
			mov al,0		; drive A:
			mov bx,di		; set offset
			mov cx,1		; number of sectors to read and store
			mov dx,2		; starting sector
			int 25h
			popf			; write back flags, which are now on top of stack (caused by int 25h)
			
			mov si,0
			mov cx,512
			mov ah,2
OUTPUT:			mov bx,di		; offset of buffer			
			mov dl,[bx+si]		; output the buffer one character at a time
			cmp dl,30h
			jae GO_AHEAD
			mov dl,"."
GO_AHEAD:		int 21h
			inc si
			LOOP OUTPUT
			NEUE_ZEILE
			NEUE_ZEILE
			jmp bx;			; jump to the buffer --> code should be executable
			MSDOS
			END START