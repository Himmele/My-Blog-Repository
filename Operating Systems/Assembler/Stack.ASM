;Pass by Reference

INCLUDE MACRO.BIB

.MODEL SMALL
.STACK 256
.DATA 			
 VARIABLE DB ?

.CODE
START:			mov ax,@data
			mov ds,ax
			mov bx,OFFSET VARIABLE
			mov VARIABLE,88d
			mov dx,[bx]
			ANZEIGE_DL
			push bx
			call PASS_BY_REFERENCE
			mov dx,[bx]
			ANZEIGE_DL
			MSDOS
PASS_BY_REFERENCE	PROC
			;pop ax
			;pop bx
			;push ax
			mov bp,sp
			mov bx,[bp]+2
			mov dx,[bx]
			ANZEIGE_DL
			ret
PASS_BY_REFERENCE	ENDP
			END START