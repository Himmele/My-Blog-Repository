; Paging


; 386 Prozessor und Befehle für Protected Mode
.386p

;---------------------------------------------------------------------------------------------------------------------------

; Stack
STACKSIZE	equ 2000h
stack			segment stack
				db STACKSIZE dup (?)
stack			ends

;---------------------------------------------------------------------------------------------------------------------------

; Makros

; legt eine Struktur für einen Deskriptor an
makeDESKRIPTOR MACRO LIMIT, BASE_015, BASE_1623, ACCESS, ATTR, BASE_2431
				dw LIMIT
				dw BASE_015
				db BASE_1623
				db ACCESS
				db ATTR
				db BASE_2431
				ENDM

; wandelt übergegebenes SEGMENT:OFFSET Paar in 24 Bit lineare Adresse um und
; trägt die ermittelte Adresse als Basisadresse in den Deskriptor N der GDT ein
baseDESKRIPTOR MACRO N, SEGMENT, OFFSET
				mov ax,SEGMENT
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET
				
				mov WORD PTR [GDT+8*N+2],ax
				shr eax,16
				mov BYTE PTR [GDT+8*N+4],al
				ENDM

; legt eine 4 Byte lange Struktur an, die als Pagetable Eintrag interpretiert wird,
; erwartet das Attribut-Byte des Eintrags sowie die phys. Speicherseitennummer (PAGEFRAME)
makePAGE		MACRO PAGEFRAME, ATTR
				db ATTR
				db (PAGEFRAME AND 0Fh) shl 4 + 0000b
				dw PAGEFRAME shr 4
				ENDM

;---------------------------------------------------------------------------------------------------------------------------

data			segment USE16

pGDT			df ?

; GLOBAL DESCRIPTOR TABLE
GDT			LABEL BYTE
				; NULL-Deskriptor
				makeDESKRIPTOR 0,0,0,0,0,0
				
				; Codesegment
				makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0
				
				; Datensegment (für Rückkehr in den Real-Mode)
				makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0
				
				; Stacksegment
				makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0
				
				; Video-RAM Deskriptor (0A0000h)
				makeDESKRIPTOR 0FFFFh,0000h,0Ah,10010010b,0,0
				
				; Video-RAM Deskriptor (0B0000h)
				makeDESKRIPTOR 0FFFFh,0000h,0Bh,10010010b,0,0
GDTSIZE		equ $-GDT

; Selektoren für obige Deskriptoren
sNIL			equ 0000000000000000b
sCODE			equ 0000000000001000b
sDATA			equ 0000000000010000b
sSTACK		equ 0000000000011000b
sSCREEN		equ 0000000000100000b
sSCREEN2		equ 0000000000101000b

; PAGETABLE
; wird später an eine durch 4096 teilbare Adresse kopiert
PAGE_TABLE	LABEL BYTE													; deckt das erste MB des Speichers ab
				db 0A0h*4 dup (?)											; untere 640 KB des Speichers
				
; 0A0000h bis 0AFFFFh -> 0A0000h bis 0AFFFFh						; lineare = phys. Adresse
				makePAGE 0A0h,1											; Pageframe-Nr, Present-Bit
				makePAGE 0A1h,1
				makePAGE 0A2h,1
				makePAGE 0A3h,1
				makePAGE 0A4h,1
				makePAGE 0A5h,1
				makePAGE 0A6h,1
				makePAGE 0A7h,1
				makePAGE 0A8h,1
				makePAGE 0A9h,1
				makePAGE 0AAh,1
				makePAGE 0ABh,1
				makePAGE 0ACh,1
				makePAGE 0ADh,1
				makePAGE 0AEh,1
				makePAGE 0AFh,1

; 0B0000h bis 0BFFFFh -> 0A0000h bis 0AFFFFh						; lineare != phys. Adresse
				makePAGE 0A0h,1
				makePAGE 0A1h,1
				makePAGE 0A2h,1
				makePAGE 0A3h,1
				makePAGE 0A4h,1
				makePAGE 0A5h,1
				makePAGE 0A6h,1
				makePAGE 0AAh,1
				makePAGE 0ABh,1
				makePAGE 0ACh,1
				makePAGE 0ADh,1
				makePAGE 0AEh,1
				makePAGE 0AFh,1
				makePAGE 0ADh,1
				makePAGE 0AEh,1
				makePAGE 0AFh,1

				db (100h-0C0h)*4 dup (?)								; die oberes 256 KB des ersten MB

; für die Rückkehr in den Real-Mode
saveSS		dw ?
saveDS		dw ?

; Page-Directory und Page-Table Speicherplätze werden zur Laufzeit an einer durch 4096 teilbaren
; Adresse angelegt
PageDir		db 2000h dup (?)											; 8 KB, da somit ein 4 KB größer Speicher an einer durch 
PageTab		db 2000h dup (?)											; 4096 teilbaren Adresse gefunden werden kann
pPageDir		dw ?
pPageTab		dw ?

data			ends

;---------------------------------------------------------------------------------------------------------------------------

code			segment USE16
				assume cs:code, ds:data, ss:stack
				
; Farbpalette und Routine zum Setzen dieser Palette
include SetPal.inc

START:
				cld															; Direction-Flag löschen
				mov ax,data
				mov ds,ax
				mov saveDS,ax
				
; Ermittlung einer durch 4096 teilbaren Adresse innerhalb des Datensegment Speicherblocks "PageDir"
				mov ax,ds
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET PageDir									; EAX = phys. Adresse
				and eax,0FFFFF000h										; die unteren 12 Bits von EAX auf 0 setzen -> durch 4096 teilb.
				add eax,01000h												; 13. Bit auf 1 setzen, damit wir innerhalb von "PageDir" sind
				shr eax,4													; in Segmentadresse umwandeln
				mov pPageDir,ax											; pPageDir = Segment, beginnend an einer durch 4K teilb. Adresse

; Ermittlung einer durch 4096 teilbaren Adresse innerhalb des Datensegment Speicherblocks "PageTab"
				mov ax,ds
				and eax,0FFFFh
				shl eax,4
				add ax,OFFSET PageTab									; EAX = phys. Adresse
				and eax,0FFFFF000h										; die unteren 12 Bits von EAX auf 0 setzen -> durch 4096 teilb.
				add eax,01000h												; 13. Bit auf 1 setzen, damit wir innerhalb von "PageTab" sind
				shr eax,4													; in Segmentadresse umwandeln
				mov pPageTab,ax											; pPageTab = Segment, beginnend an einer durch 4K teilb. Adresse
				
; Adresse der Page-Table im Page-Directory eintragen
				mov di,pPageDir
				mov es,di													; Segment des Page-Directory in ES
				xor edi,edi													; EDI auf 0 setzen --> ES:EDI zeigen auf Anfang des Page-Direct.
				
				movzx eax,pPageTab					; Segmentadresse von pPageTab nach EAX und die restlichen Bits mit 0 auffüllen
				shl eax,4													; auf 20 Bit verbreitern (untere 12 Bits sind 0)
; EAX enthält nun die durch 4096 teilbare phys. Adresse der Page-Table
				add eax,1													; Present-Bit auf 1 setzen
				stosd															; überträgt EAX nach ES:EDI -> pPageTab nach pPageDir+0


; keine Unterschiede zwischen linearen und phys. Adressen im Bereich 0 bis 09FFFFh (untere 640 KB)
				mov di,SEG PAGE_TABLE
				mov es,di
				xor edi,edi
				mov di,OFFSET PAGE_TABLE								; Page-Table in ES:EDI
				
				mov ebx,0													; Page-Nummer der ersten Frame
				mov ecx,0A0h												; 160 Schleifendurchläufe, da 160 * 4 KB = 640 KB
				
FIRST_PART:
				mov eax,ebx													; Page-Frame-Nummer nach EAX
				shl eax,12													; Pageframe X auf phys. Adresse X * 2^12 (4096)
				add eax,1													; Present-Bit setzen
				stosd															; überträgt EAX nach ES:EDI und inkrementiert EDI um 4
				
				inc ebx
				loop FIRST_PART
				
; keine Unterschiede zwischen linearen und phys. Adressen im Bereich 0C0000h bis 0FFFFF (768 KB bis 1024 KB)
				mov edi,0C0h*4												; Offset innerhalb der Page-Table
				add di,OFFSET PAGE_TABLE								; Offset der Page-Table
				mov ebx,0C0h												; Page-Nummer der "ersten" Frame (192, da 192*4KB=768KB)
				mov ecx,0100h-0C0h										; 64 Schleifendurchläufe, da 64 * 4 KB = 256 KB
			
SECOND_PART:
				mov eax,ebx													; Page-Frame-Nummer nach EAX
				shl eax,12													; Pageframe X auf phys. Adresse X * 2^12 (4096)
				add eax,1													; Present-Bit setzen
				stosd															; überträgt EAX nach ES:EDI und inkrementiert EDI um 4
				
				inc ebx
				loop SECOND_PART

; Page-Table an den vorher ermittelten, durch 4096 teilbaren Speicherplatz kopieren
				mov si, OFFSET PAGE_TABLE								; Source-Index
				mov di, pPageTab											; Segment der Page-Table
				mov es,di													; Page-Table nach ES
				xor edi,edi													; EDI auf 0
				
				mov ecx,4096												; 4096 Schleifendurchläufe -> 4096 Bytes kopieren
				rep movsb													; kopiert ein Byte von DS:SI nach ES:DI, inkrement. SI und DI

; Stacksegment für Rückkehr in den Real-Mode sichern
				mov ax,ss
				mov saveSS,ax

; Grafikmodus 320X200 Pixel bei 256 Farben (1 Pixel entspr. 8 Bit im linearen Speicher)
				mov ax,13h
				int 10h
				call SetPal

; Deskriptoren initialisieren
				baseDESKRIPTOR 1, cs, 0
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

; alle Interrupts aus, nicht alle Interrupts können behandelt werden, es wurden keine Handler bereitgestellt
				cli
				;in al,70h													; ???
				;or al,80h													; ???
				;out 70h,al													; ???
				
; für Rücksprung, Codesegment-Adresse unter RMODE_CS sichern
				mov ax,cs
				mov cs:RMODE_CS,ax
				
; Protected Mode aktivieren
				mov eax,cr0
				or eax,1
				mov cr0,eax
				
; "FAR-JMP" zu Selektor:Offset
				db 0eah
				dw OFFSET ProtectedMode
				dw sCODE

ProtectedMode:
; Selektor für Stack Segment setzen
				mov ax,sSTACK
				mov ss,ax
				mov esp,STACKSIZE

; Schattenregister der anderen Segmente initialisieren
				mov ax,sDATA
				mov ds,ax
				mov es,ax
				mov fs,ax
				mov gs,ax
				
; CR3 initialisieren, Adresse des Page-Directory eintragen
				mov ax,pPageDir
				and eax,0FFFFh
				shl eax,4														; Adresse auf 20 Bit verbreitern (untere 12 Bits = 0)
				mov cr3,eax									; CR3 enthält die oberen 20 Bits der phys. Basisadresse des Page-Directory
				
; Paging aktivieren: PG Bit (Bit 31) im CR0 Register setzen
				mov ebx,cr0
				or ebx,80000000h
				mov cr0,ebx

; Farbverlauf auf VGA-Screen kopieren
				mov ax,sSCREEN
				mov es,ax
				xor edi,edi														; ES:EDI -> 0A0000h:0 -> 0A0000h:0
				
				mov edx,200														; 200 Zeilen
				mov eax,0														; Farbwert
				
AGAIN:
				mov ecx,320/4
				rep stosd														; überträgt EAX in ES:EDI und erhöht EDI um 4
																					; schreibt Farbwert für 4 Pixel (4 Byte) in den Speicher
				add eax,01010101h												; nächster Farbindex
				dec edx															; wieder eine Zeile weniger
				jnz AGAIN

; warten auf ESC
; Da die Interrupts ausgeschaltet wurden, wird der Tastaturpuffer nicht aktualisiert
; Aus diesem Grund wird der Tastaturstatus direkt abgefragt und solange gewartet bis 
; die ESC Taste (Scancode 1) gedrückt wird

WAITKEY:
				in al,[60h]
				cmp al,1
				jne WAITKEY

; Farbverlauf nach 0B0000h kopieren -> da dieser Bereich ins VGA-RAM gemappt wurde, erscheint es auf dem Bildschirm
				mov ax,sSCREEN2
				mov es,ax
				xor edi,edi														; ES:EDI -> 0B0000h:0 -> 0A0000h:0
				
				mov edx,200														; 200 Zeilen
				mov eax,0														; Farbwert

AGAIN2:
				mov ecx,320/4
				rep stosd
				
				add eax,01010101h
				dec edx
				jnz AGAIN2
				
; warten bis ESC losgelassen wurde

RELEASEKEY:
				in al,[60h]
				cmp al,81h
				jne RELEASEKEY

; Paging ausschalten
				mov eax,cr0
				and eax,7FFFFFFFh
				mov cr0,eax
				
				mov eax,0
				mov cr3,eax

; zurück zum Real-Mode
				mov ax,sDATA
				mov ds,ax
				mov es,ax
				mov fs,ax
				mov gs,ax
				
; Protected Mode ausschalten
				mov eax,cr0
				and eax,not 1
				mov cr0,eax
				
				db 0eah
				dw OFFSET RealMode
RMODE_CS		dw ?

RealMode:
				mov ax,saveSS
				mov ss,ax
				mov sp,STACKSIZE
				
				;in al,70h														; ???
				;or al,not 80h													; ???
				;out 70h,al														; ???
				sti
				
; auf Taste warten
				mov ah,0
				int 16h
				
; Textmodus initialisieren
				mov ax,3
				int 10h
				
; zurück zu DOS
				mov ax,4C00h
				int 21h
code			ends
				end START