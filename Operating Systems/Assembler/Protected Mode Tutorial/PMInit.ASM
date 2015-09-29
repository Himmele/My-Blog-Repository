; use TASM and TLINK
; kooperatives Multitasking mit zwei Tasks - Initialisierungs-Task
; (2 getrennte Codesegmente sowie gemeinsam genutzte Stack- und Datensegmente)

; 386 Prozessor und Befehle für den Protected Mode
.386p

;---------------------------------------------------------------------------------------------------------------------------
; PM_Init als FAR Prozedur deklarieren (für Linker)

public				PM_Init

;---------------------------------------------------------------------------------------------------------------------------
; Konstante
BELL			equ 7

;---------------------------------------------------------------------------------------------------------------------------
; Selektoren

sNIL					equ 0000000000000000b
sCODE					equ 0000000000001000b
sCODE_RETURN		equ 0000000000010000b
sDATA					equ 0000000000011000b
sSTACK				equ 0000000000100000b
sSCREEN				equ 0000000000101000b
sTASK_MAIN			equ 0000000000110000b
sTASK_INIT			equ 0000000000111000b

;---------------------------------------------------------------------------------------------------------------------------
; Strukturen

GDTIDTSTRUC			STRUC
	limit				dw ?
	base				dd ?
GDTIDTSTRUC			ENDS

TSS_STRUC			STRUC
	tbacklink		dw ?									; Backlink-Field: TSS-Selektor des vorherigen Tasks
						dw 0
	tesp0				dd ?									; Stackzeiger für Aufrufe von Routinen in Ring 0
	tss0				dw ?									; Stack für Aufrufe von Routinen in Ring 0
						dw 0
	tesp1				dd ?									; Stackzeiger für Aufrufe von Routinen in Ring 1
	tss1				dw ?									; Stack für Aufrufe von Routinen in Ring 1
						dw 0
	tesp2				dd ?									; Stackzeiger für Aufrufe von Routinen in Ring 2
	tss2				dw ?									; Stack für Aufrufe von Routinen in Ring 2
						dw 0
	tcr3				dd ?									; Zeiger auf Page-Directory des Tasks
	teip				dd ?									; Instruction Pointer
	teflags			dd ?
	teax				dd ?
	tecx				dd ?
	tedx				dd ?
	tebx				dd ?
	tesp				dd ?
	tebp				dd ?
	tesi				dd ?
	tedi				dd ?
	tes				dw ?
						dw 0
	tcs				dw ?
						dw 0
	tss				dw ?
						dw 0
	tds				dw ?
						dw 0
	tfs				dw ?
						dw 0
	tgs				dw ?
						dw 0
	tldtr				dw ?									; Selektor des LDT des Tasks
						dw 0
	tt					dw ?							; Trap on Task: löst, wenn gesetzt, bei Task-Switch "Debug-Fault" Exception aus
	tiopbb			dw ?									; Offset der I/O Permission Bitmap innerhalb des TSS
TSS_STRUC			ENDS

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
				dw SELEKTOR									; falls dies ein Selektor auf ein TSS-Segment ist, wird der entsprechende
																; Interrupt in einem eigenen Task ausgeführt (siehe QNX)
				db 0														; unbenutzt
				db ATTR													; siehe unten
				dw 0														; Offset in Code-Segment (für 32 Bit)
				ENDM

;---------------------------------------------------------------------------------------------------------------------------
data			segment USE16 PUBLIC									; PUBLIC: Segmente mit gleichem Namen werden vom Linker zu einem 
																			; einzigen Segment zusammengebunden

pGDT			GDTIDTSTRUC ?
pIDT			GDTIDTSTRUC ?

; TSS für Initialisierungs-Task (dieser Task)
TSS_INIT_TASK		TSS_STRUC ?

; TSS für Haupt-Task
TSS_MAIN_TASK		TSS_STRUC ?
TSS_SIZE				equ $-TSS_INIT_TASK

; Global Descriptor Table
GDT			LABEL BYTE
				; Null-Deskriptor
				makeDESKRIPTOR 0,0,0,0,0,0
						
				; Codesegment (Initialisierungs-Task)
				makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0
				
				; Codesegment für Rücksprung (Haupt-Task)
				makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0
						
				; gemeinsames Datensegment
				makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0
						
				; gemeinsames Stacksegment
				makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0
						
				; Video-RAM
				makeDESKRIPTOR 4000,8000h,0Bh,10010010b,0,0
						
				; TSS-Deskriptor für Haupt-Task
				makeDESKRIPTOR TSS_SIZE,?,?,10001001b,0,0			; Systemsegmentdeskriptor: 386 TSS-Deskriptor
						
				; TSS-Deskriptor für Initialisierungs-Task		; Systemsegmentdeskriptor: 386 TSS-Deskriptor
				makeDESKRIPTOR TSS_SIZE,?,?,10001001b,0,0
GDTSIZE		equ $-GDT

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

				assume cs:PMCode,ds:data
PMCode		segment USE16

align 4																	; Segment MUSS an einer durch 16 teilbaren Adresse beginnen
PM_Init		PROC FAR
				cld
				
				mov ax,data
				mov ds,ax
				mov es,ax
				mov saveDS,ax
				
; Stacksegment sichern
				mov ax,ss
				mov saveSS,ax

; Rücksprungadresse vom Stack holen und für Rücksprung sichern (wird von CALL auf den Stack gebracht)
				pop bx													; Offset
				pop cx													; Segmentadresse
				baseDESKRIPTOR 2, cx, 0								; für Rücksprung Basisadresse im Deskriptor eintragen

; andere Basisadressen initialisieren
				baseDESKRIPTOR 1, cs, 0
				baseDESKRIPTOR 3, ds, 0
				baseDESKRIPTOR 4, ss, 0
				
				mov ax,ds
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET TSS_MAIN_TASK						; phys. Adresse des TSS-Segments für den Haupt-Task
				mov WORD PTR [GDT+8*6+2],ax						; in GDT eintragen (Deskriptor 6)
				shr eax,16
				mov BYTE PTR [GDT+8*6+4],al
				
				mov ax,ds
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET TSS_INIT_TASK						; phys. Adresse des TSS-Segments für den Initialisierungs-Task
				mov WORD PTR [GDT+8*7+2],ax						; in GDT eintragen (Deskriptor 7)
				shr eax,16
				mov BYTE PTR [GDT+8*7+4],al
				
; Task State Segment des Haupt-Tasks MUSS von Hand initialisiert werden
; -> TSS des Initialisierungs-Task wird duch Taskwechsel zum Haupt-Task vom Prozessor automatisch initialisiert
				mov di,OFFSET TSS_MAIN_TASK						; DS:DI zeigt auf Beginn des TSS des Initialisierungs-Tasks
				
				mov eax,esp
				and eax,0FFFFh											; EAX = SP
				
				mov [di+tbacklink],0
				mov [di+tesp0],eax									; wird auf aktuellen SP Wert gesetzt
				mov [di+tss0],sSTACK
				mov [di+tesp1],eax
				mov [di+tss1],sSTACK
				mov [di+tesp2],eax
				mov [di+tss2],sSTACK
				mov [di+tcr3],0
				
				and ebx,0FFFFh											; BX enthält Offset des Codesegments
				mov [di+teip],ebx
				
				pushfd													; Erweiterte Flags auf den Stapel bringen
				pop ebx													; Erweiterte Flags vom Stapel nehmen
				mov [di+teflags],ebx
				mov [di+teax],sTASK_INIT							; für Rücksprung (zum Ausschalten des Protected Mode)
				mov [di+tecx],0
				mov [di+tedx],0
				mov [di+tebx],0
				
				mov [di+tesp],eax
				mov [di+tebp],0
				mov [di+tesi],0
				mov [di+tedi],0
				mov [di+tes],sSCREEN									; zur Ausgabe des Strings
				mov [di+tcs],sCODE_RETURN							; eigenes Codesegment
				mov [di+tss],sSTACK
				mov [di+tds],sDATA
				mov [di+tfs],sDATA
				mov [di+tldtr],0										; kein LDT
				mov [di+tt],0
				mov [di+tiopbb],0										; kein I/O Permission Bitmap
				
; Adresse der GDT setzen
				mov ax,GDTSIZE
				mov [pGDT.limit],ax
				
				mov ax,SEG GDT
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET GDT
				mov [pGDT.base],eax
				
				lgdt pGDT

; Interrupts aus, IDT wird verändert
				cli

; Adresse der IDT setzen
				mov ax,IDTSIZE
				mov [pIDT.limit],ax
				
				mov ax,SEG IDT
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET IDT
				mov [pIDT.base],eax
				
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

; Datenregister vorbereiten --> Schattenregister initialisieren
				mov ax,sDATA
				mov ds,ax
				mov es,ax
				mov fs,ax
				mov gs,ax
				
				mov ax,sTASK_INIT								; TR auf aktuelles TSS setzen, damit beim Task-Switch die Informationen
																	; vom aktuellen Task vom Prozessor dort automatisch abgelegt werden
				ltr ax

; "FAR-JMP" zum Haupt-Task (TASK-WECHSEL)
				db 0eah
				dw 0
				dw sTASK_MAIN
				
; nachdem der Haupt-Task wieder zu diesem Task wechselt, landet der Prozessor wieder hier und setzt seine Arbeit fort
				
;---------------------------------------------------------------------------------------------------------------------------

; System Call Handler aufrufen
;				int 25h
				
;---------------------------------------------------------------------------------------------------------------------------

; sinnlosen Selektor laden, endet in Exception 13
;				mov ax,1234h
;				mov ds,ax

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
				mov [pIDT.limit],03FFh
				mov [pIDT.base],0
				
				lidt pIDT													; IDT laden und aktivieren (sti)
				
; Stack Segmentregister auf alten Wert
				mov ax,saveSS
				mov ss,ax
				mov sp,01000h												; 4 KB Stack
				
; zurück zu DOS
				mov ax,4C00h
				int 21h
				
				ENDP

;---------------------------------------------------------------------------------------------------------------------------

; Exception Handler für Exceptions 0 bis 17

include ShowHex.inc

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
				mov [pIDT.limit],03FFh
				mov [pIDT.base],0
				
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
				ends
				end