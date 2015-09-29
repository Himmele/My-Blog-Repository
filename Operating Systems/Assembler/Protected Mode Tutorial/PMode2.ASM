; enter protected mode, enable GDT and IDT, print a message, get an exception, exit to real mode

;---------------------------------------------------------------------------------------------------------------------------

; 386 Prozessor unb Befehle für Protected Mode
				.386p

;---------------------------------------------------------------------------------------------------------------------------

; Stack
STACKSIZE	equ 2000h												; 8 KB Stack
stack			segment stack
				db STACKSIZE dup (?)
stack			ends

;---------------------------------------------------------------------------------------------------------------------------

; Konstante
BELL			equ 7

;---------------------------------------------------------------------------------------------------------------------------

; Makros

; legt eine Struktur für eines Deskriptor an
makeDESKRIPTOR MACRO LIMIT, BASE_015, BASE_1623, ACCESS, ATTR, BASE_2431
				dw LIMIT
				dw BASE_015
				db BASE_1623
				db ACCESS
				db ATTR
				db BASE_2431
				ENDM

; wandelt übergeordnetes SEGMENT:OFFSET Paar in 24 Bit phys. Adresse um und
; trägt diese Adresse als Basisadresse in den Deskriptor N der GDT ein
baseDESKRIPTOR MACRO N, SEGMENT, OFFSET
				mov ax,SEGMENT
				and eax,0FFFFh
				shl eax,4
				add eax,OFFSET
				
				mov word ptr [GDT+8*N+2],ax
				shr eax,16
				mov byte ptr [GDT+8*N+4],al
				ENDM

; legt eine Struktur für ein Interrupt-Gate an
makeIDT		MACRO OFFSET, SELEKTOR, ATTR
				dw OFFSET												; Offset in Code-Segment
				dw SELEKTOR												; Code-Segment Selektor
				db 0														; unbenutzt
				db ATTR													; siehe unten
				dw 0														; Offset in Code-Segment (für 32 Bit)
				ENDM

;---------------------------------------------------------------------------------------------------------------------------

data			segment USE16

pGDT			df ?
pIDT			df ?

; GLOBAL DESKRIPTOR TABLE
GDT			LABEL BYTE
				makeDESKRIPTOR 0,0,0,0,0,0							; NULL-Deskriptor
				makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0		; Codesegment
				makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0		; Datensegment
				makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0		; Stacksegment
				makeDESKRIPTOR 4000,8000h,0Bh,10010010b,0,0	; Video-RAM
GDTSIZE		equ $-GDT

; Selektoren für obige Deskriptoren
sNIL			equ 0000000000000000b
sCODE			equ 0000000000001000b
sDATA			equ 0000000000010000b
sSTACK		equ 0000000000011000b
sSCREEN		equ 0000000000100000b

; INTERRUPT DESKRIPTOR TABLE
IDT			LABEL BYTE
				makeIDT IRQ_00, sCODE, 10001110b					; ATTR = P|DPL|S|TYPE
																			; S=0: Systemsegmentdesktiptor -> TYPE Bits geben Deskriptorart an
																			; hier 386 Interrupt-Gate Deskriptor (Tabelle 2.3)
				makeIDT IRQ_01, sCODE, 10001110b
				makeIDT IRQ_02, sCODE, 10001110b
				makeIDT IRQ_03, sCODE, 10001110b
				makeIDT IRQ_04, sCODE, 10001110b
				makeIDT IRQ_05, sCODE, 10001110b
				makeIDT IRQ_06, sCODE, 10001110b
				makeIDT IRQ_07, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_09, sCODE, 10001110b
				makeIDT IRQ_10, sCODE, 10001110b
				makeIDT IRQ_11, sCODE, 10001110b
				makeIDT IRQ_12, sCODE, 10001110b
				makeIDT IRQ_13, sCODE, 10001110b
				makeIDT IRQ_14, sCODE, 10001110b
				makeIDT IRQ_15, sCODE, 10001110b
				makeIDT IRQ_16, sCODE, 10001110b
				makeIDT IRQ_17, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT IRQ_08, sCODE, 10001110b
				makeIDT SYS_CALL, sCODE, 10001111b				; int 37 (25h) ist System Call Handler; 386 Trap Deskriptor				
IDTSIZE		equ $-IDT

; für Rückkehr in den Real-Mode
saveSS		dw ?
saveDS		dw ?

; für Ausgabe der allgemeinen Register
r_eax			dd ?
r_ebx			dd ?
r_ecx			dd ?
r_edx			dd ?
r_esi			dd ?
r_edi			dd ?
r_esp			dd ?
r_ebp			dd ?
r_eip			dd ?
r_cs			dw ?
r_ds			dw ?
r_es			dw ?
r_fs			dw ?
r_gs			dw ?
r_ss			dw ?

exception	db ?

; ASCII Beschreibung der auftretenden Exceptions
Exception_Names LABEL BYTE
				db "(Fehler bei Division)               "
Eintrag_Laenge equ $-Exception_Names
				db "(Debugger-Interrupt)                "
				db "(NMI - Non-Maskable Interrupt)      "
				db "(Breakpoint)                        "
				db "(Überlauf (INTO))                   "
				db "(Arraygrenzen überschritten (BOUND))"
				db "(ungültiger Opcode)                 "
				db "(Coprozessor nicht installiert)     "
				db "(Doppelfehler)                      "
				db "(Segmentüberlauf des Coprozessors   "
				db "(TSS ungültig)                      "
				db "(Segment nicht im Hauptspeicher)    "
				db "(Stacküberlauf)                     "
				db "(allgemeine Schutzverletzung)       "
				db "(Paging Ausnahmezustand)            "
				db "(reserviert)                        "
				db "(Coprozessor-Fehler)                "
				db "(Ausrichtungsfehler)                "

; Exception Text, wird nach der Rückschaltung in den Real-Mode über die
; Funktion 09h des DOS Interrupts 21h ausgegeben
Text			db BELL, "Exception "
oNR			db "00 "
oNAME			db Eintrag_Laenge dup (' ')
				db 13, 10, 10											; CR, LF, LF
				db "CS = "
oCS			db "0000     "
				db "EIP= "
oEIP			db "00000000 ", 13, 10
				db "SS = "
oSS			db "0000     "
				db "ESP= "
oESP			db "00000000 ", 13, 10
				db "EAX= "
oEAX			db "00000000 "
				db "EBX= "
oEBX			db "00000000 "
				db "ECX= "
oECX			db "00000000 "
				db "EDX= "
oEDX			db "00000000 ", 13, 10
				db "ESI= "
oESI			db "00000000 "
				db "EDI= "
oEDI			db "00000000 "
				db "EBP= "
oEBP			db "00000000 ", 13, 10
				db "DS = "
oDS			db "0000     "
				db "ES = "
oES			db "0000     "
				db "FS = "
oFS			db "0000     "
				db "GS = "
oGS			db "0000     ", 13, 10
				db "$"

nul			dd ?														; zusätzlich zur Fehleradresse (CS:EIP) legt der Prozessor einen
																			; 32 Bit Fehlercode auf dem Stack ab --> entfernen mit POP nul

String		db "Handling a System Call in Protected Mode!"
StrLength	equ $-String

data			ends

;---------------------------------------------------------------------------------------------------------------------------

code			segment USE16
				assume cs:code, ds:data, ss:stack
				
include ShowHex.inc													; zur Ausgabe der Register als Hex-Werte

START:
				mov ax,data
				mov ds,ax
				mov saveDS,ax
				
				mov ax,ss
				mov saveSS,ax
				
; Basisadressen der GDT Deskriptoren initialisieren
				baseDESKRIPTOR 1, cs, 0								; Deskriptor-Nr, Segmentadresse, Offset
				baseDESKRIPTOR 2, ds, 0
				baseDESKRIPTOR 3, ss, 0
				
; Adresse der GDT setzen
				mov ax,GDTSIZE
				mov WORD PTR [pGDT+0],ax
				
				mov ax,SEG GDT
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET GDT
				mov DWORD PTR [pGDT+2],eax
				
				lgdt pGDT

; Interrupts aus, IDT wird verändert
				cli

; Adresse der IDT setzen
				mov ax,IDTSIZE
				mov WORD PTR [pIDT+0],ax
				
				mov ax,SEG IDT
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET IDT
				mov DWORD PTR [pIDT+2],eax
				
				lidt pIDT												; IDT laden und aktivieren (sti)
				
; für Rücksprung in Real Mode Codesegment-Adresse unter RMODE_CS sichern
				mov ax,cs
				mov cs:RMODE_CS,ax
				mov cs:RMODE_CS2,ax
				
; Protected Mode aktivieren
				mov eax,cr0
				or eax,1
				mov cr0,eax

; "FAR-JMP" zu Selektor:Offset
				db 0eah
				dw OFFSET ProtectedMode
				dw sCODE
				
ProtectedMode:
; Stack Segment setzen
				mov ax,sSTACK
				mov ss,ax
				mov esp,STACKSIZE

; Datenregister vorbereiten --> Schattenregister initialisieren
				mov ax,sDATA
				mov ds,ax
				mov es,ax
				mov fs,ax
				mov gs,ax
				
;---------------------------------------------------------------------------------------------------------------------------

; System Call Handler aufrufen
				int 25h
				
;---------------------------------------------------------------------------------------------------------------------------

; sinnlosen Selektor laden, endet in Exception 13
				mov ax,1234h
				mov ds,ax

;---------------------------------------------------------------------------------------------------------------------------

; zurück zum Real-Mode

BACK_TO_REALMODE:
				cli
				mov ax,sDATA
				mov ds,ax
				mov es,ax
				mov fs,ax
				mov gs,ax
				mov ss,ax
				
				mov eax,cr0
				and eax,not 1
				mov cr0,eax
				
; "FAR-JMP" zum Real-Mode
				db 0eah
				dw OFFSET RealMode
RMODE_CS		dw ?

RealMode:
; Interrupt-Deskriptortabelle wieder auf Realmode-Standard
				mov WORD PTR [pIDT+0],03FFh
				mov DWORD PTR [pIDT+2],0
				
				lidt pIDT													; IDT laden und aktivieren (sti)
				
; Stack Segmentregister auf alten Wert
				mov ax,saveSS
				mov ss,ax
				mov sp,01000h												; 4 KB Stack
				
; zurück zu DOS
				sti
				mov ax,4C00h
				int 21h

;---------------------------------------------------------------------------------------------------------------------------

; Exception Handler für Exceptions 0 bis 17

IRQ_00:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,0														; Exception Nr
				jmp QUIT_ALL
				
IRQ_01:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,1														; Exception Nr
				jmp QUIT_ALL
				
IRQ_02:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,02													; Exception Nr
				jmp QUIT_ALL
				
IRQ_03:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,3														; Exception Nr
				jmp QUIT_ALL
				
IRQ_04:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,4														; Exception Nr
				jmp QUIT_ALL
				
IRQ_05:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,5														; Exception Nr
				jmp QUIT_ALL
				
IRQ_06:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,6														; Exception Nr
				jmp QUIT_ALL
				
IRQ_07:																		; CS und EIP sind schon auf dem Stack
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,7														; Exception Nr
				jmp QUIT_ALL
				
IRQ_08:																		; Clock Interrupt
				jmp BACK_TO_REALMODE

IRQ_09:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,9														; Exception Nr
				jmp QUIT_ALL
				
IRQ_10:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,10													; Exception Nr
				jmp QUIT_ALL
				
IRQ_11:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,11													; Exception Nr
				jmp QUIT_ALL
				
IRQ_12:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,12													; Exception Nr
				jmp QUIT_ALL
				
IRQ_13:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,13													; Exception Nr
				jmp QUIT_ALL
				
IRQ_14:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,14													; Exception Nr
				jmp QUIT_ALL
				
IRQ_15:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,15													; Exception Nr
				jmp QUIT_ALL
				
IRQ_16:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,16													; Exception Nr
				jmp QUIT_ALL
				
IRQ_17:																		; CS, EIP und 32-Bit Fehlercode sind schon auf dem Stack
				pop nul														; Fehlercode vom Stack nehmen
				push esp														; ESP auf den Stack
				push ds														; DS auf den Stack
				push eax
				mov ax,sDATA
				mov ds,ax
				pop eax
				mov r_eax,eax												; EAX sichern
				mov al,17													; Exception Nr
				jmp QUIT_ALL

SYS_CALL:
				push ax
				push es
				push cx
				push si
				push di
				
				mov ax,sSCREEN
				mov es,ax
				mov cx,StrLength
				mov si,OFFSET String
				mov di,(80*2)*24+(80-StrLength)*2
SHOWSTRING: mov al,[si]
				stosb
				inc si
				inc di
				loop SHOWSTRING
				
				pop di
				pop si
				pop cx
				pop es
				pop ax
				
				iret															; CS:EIP vom Stack nehmen und zurück zum Aufrufer
				
;---------------------------------------------------------------------------------------------------------------------------

; Abbruch durch Exception, Rückkehr in den Real-Mode und Ausgabe aller Register

QUIT_ALL:
				mov exception,al
				pop ax
				mov r_ds,ax
				
				pop eax
				mov r_esp,eax
				
				pop eax
				mov r_eip,eax
				
				pop eax
				mov r_cs,ax
				
				pop eax
				
				mov r_ebx,ebx
				mov r_ecx,ecx
				mov r_edx,edx
				mov r_esi,esi
				mov r_edi,edi
				mov r_ebp,ebp
				mov r_es,es
				mov r_fs,fs
				mov r_gs,gs
				mov r_ss,ss

; zurück in den Real-Mode
				cli
				
				mov ax,sDATA
				mov ds,ax
				mov es,ax
				mov fs,ax
				mov gs,ax
				mov ss,ax
				
				mov eax,cr0
				and eax,not 1
				mov cr0,eax
				
; "FAR-JMP" in den Real-Mode
				db 0eah
				dw OFFSET RMODE2
RMODE_CS2	dw ?

RMODE2:
; Interrupt-Deskriptortabelle wieder auf Real-Mode Standard
				mov WORD PTR [pIDT+0],03FFh
				mov DWORD PTR [pIDT+2],0
				
				lidt pIDT													; IDT setzen und aktivieren (sti)

; Stack Segmentregister auf alten Wert
				mov ax,saveSS
				mov ss,ax
				mov sp,01000h
				
				mov ax,saveDS
				mov ds,ax
				mov es,ax

; Umwandlung der Registerwerte in hexadez. ASCII Werte
				mov al,exception											; auszugebende Zahl in AL
				mov ah,0
				mov di,OFFSET oNR											; Ziel-Offset in DI (ES:DI)
				call ShowHexByte
				
				mov eax,r_eax												; auszugebende Zahl in EAX
				mov di,OFFSET oEAX
				call ShowHexDword											; Ziel-Offset in DI (ES:DI)
				
				mov eax,r_ebx
				mov di,OFFSET oEBX
				call ShowHexDword
				
				mov eax,r_ecx
				mov di,OFFSET oECX
				call ShowHexDword
				
				mov eax,r_edx
				mov di,OFFSET oECX
				call ShowHexDword
				
				mov eax,r_esi
				mov di,OFFSET oESI
				call ShowHexDword
				
				mov eax,r_edi
				mov di,OFFSET oESI
				call ShowHexDword
				
				mov eax,r_ebp
				mov di,OFFSET oEBP
				call ShowHexDword
				
				mov eax,r_esp
				mov di,OFFSET oESP
				call ShowHexDword
				
				mov eax,r_eip
				mov di,OFFSET oEIP
				call ShowHexDword
				
				mov ax,r_cs
				mov di,OFFSET oCS
				call ShowHexWord
				
				mov ax,r_ds
				mov di,OFFSET oDS
				call ShowHexWord
				
				mov ax,r_es
				mov di,OFFSET oES
				call ShowHexWord
				
				mov ax,r_fs
				mov di,OFFSET oFS
				call ShowHexWord
			
				mov ax,r_gs
				mov di,OFFSET oGS
				call ShowHexWord
				
				mov ax,r_ss
				mov di,OFFSET oSS
				call ShowHexWord
				
				mov al,exception											; Exception Nr
				and ax,0FFh													; nur in AL
				mov bl,Eintrag_Laenge
				mul bl														; AX = AL * BL
				add ax,OFFSET Exception_Names		; durch addieren von Exception_Names: Offset des Eintrags in Exception_Names
				mov si,ax													; Offset als Source Index verwenden
				mov di,OFFSET oNAME										; Ziel-Offset
				mov cx,Eintrag_Laenge									; Anzahl der Schleifendurchläufe
				rep movsb													; movsb: kopier ein Byte von DS:SI nach ES:DI
				
; Ausgabe der Exception
				mov dx,OFFSET Text
				mov ah,09h													; Zeichenkette in DS:DX zur Anzeige bringen (Terminator = '$')
				int 21h

; zurück zu DOS
				mov ax,4C00h
				int 21h
code			ends
				end START