; enable Gate A20 to use full 4 GB address space

;	1) anlegen einer GDT, sowie Laden des GDT-Registers mit der Adresse der GDT
;	2) Bit 0 (PE-Bit) im Register CR0 setzen, um Protected Mode zu aktivieren
;	3) durchführen eines "FAR-JMP" um die Warteschlange zu leeren und das  
;		Selektorregister CS (sowie das Deskriptorschattenregister) mit einem
;		gültigen Wert zu initialisieren
;	4) direkte Ausgabe eines Strings in den Bildschirmspeicher
;	5)	Rückkehr in den Realmode

;---------------------------------------------------------------------------------------------------------------------------

; 386 Prozessor und Befehle für Protected Mode
.386p																; 386 Mode

;---------------------------------------------------------------------------------------------------------------------------

; Stack
stack		segment stack
			dw 256 dup (?)										; 256 Byte Stack
stack		ends

;---------------------------------------------------------------------------------------------------------------------------

; Daten
data		segment USE16										; 16 Bit Segmentgröße (64 KB)
			
pGDT 		df ?													; Zeiger auf GDT, 2 Byte Größe, 4 Byte phys. Basisadresse
			
; GLOBAL DESKRIPTOR TABLE
GDT		LABEL Byte											; GDT als Oberbegriff der nachfolgenden Variablen --> der kompletten GDT

; Deskriptor 0 wird nicht benutzt
des_dummy	db 8 dup (0)

; Deskriptor 1, Codesegment Deskriptor
des_code		dw 0FFFFh										; Segmentgröße: 64K, da Granularity auf 0
				dw ?												; Basisadresse --> wird zur Laufzeit eingetragen
				db ?												; Basisadresse --> wird zur Laufzeit eingetragen
				db 10011010b									; Present-Bit = 1, DPL = 0, ist Codesegment
				db 00000000b									; Granularity = 0, 286 kompatibles Codesegment (max. 64K)
				db 0												; Segmentgröße
				
; Deskriptor 2, VIDEO RAM (0B800h)
des_vram		dw 4000											; Segmentgröße: 4000 Byte
				dw 8000h											; phys. Basisadresse (B800 SHL 4)
				db 0Bh											; phys. Basisadresse (B800 SHL 4)
				db 10010010b									; Present-Bit = 1, DPL = 0, ist Datensegment
				db 00000000b									; Granularity = 0, 286 kompatibles Datensegment (max. 64K)
				db 0												; Segmentgröße
				
; Deskriptor 3, entspr. akt. Real-Mode Datensegment
des_data		dw 0FFFFh		
				dw ?
				db ?
				db 10010010b
				db 00000000b
				db 0

; Deskriptor 4, 386 Datensegment mit 16 MB
des_data2   dw 0FFFh											; 16 MB groß
				dw 0000h											; beginnt bei 1 MB
				db 10h
				db 10010010b
				db 11000000b									; Granularity auf 4 KB, 386 Datensegment > 64 KB
				db 0

GDTSIZE		equ $-GDT

String		db "Hello from Protected Mode!"
StrLength	equ $-string

data		ends

;---------------------------------------------------------------------------------------------------------------------------

code		segment USE16										; 16 Bit Segmentgröße (64 KB)
			assume cs:code, ds: data, ss:stack

START:	mov ax,data
			mov ds,ax
			
; Codesegment Basisadresse in Deskriptor 1 eintragen
			mov ax,cs											; Segmentadresse des Codesegments
			and eax,0FFFFh										; obere 16 Bits von EAX explizit auf 0 setzen
			shl eax,4											; Segmentadresse auf 20 Bit verbreitern,
			add ax,OFFSET START								; Offset addieren --> phys. Adresse
			
			mov WORD PTR [des_code+2],ax					; untere 16 Bits in Deskriptor schreiben
			shr eax,16											; Zugriff auf die oberen 16 Bits mittels AX
			mov BYTE PTR [des_code+4],al					; obere 8 Bits in Deskriptor schreiben

; Datensegment Basisadresse in Deskriptor 3 eintragen
			mov ax,ds											; Segmentadresse des Datensegments
			and eax,0FFFFh										; obere 16 Bits von EAX explizit auf 0 setzen
			shl eax,4											; Segmentadresse auf 20 Bits verbreitern --> phys. Adresse
			
			mov WORD PTR [des_data+2],ax
			shr eax,16
			mov BYTE PTR [des_data+4],al

; Adresse und Größe der GDT in pGDT eintragen
			mov ax,GDTSIZE
			mov WORD PTR [pGDT+0],ax						; Größe der GDT in untere 2 Byte von pGDT schreiben
			
			mov ax,SEG GDT										; Segmentadresse des Segments von GDT ermitteln und in AX schreiben
			and eax,0FFFFh										; obere 16 Bits von EAX explizit auf 0 setzen
			shl eax,4											; Segmentadresse auf 20 Bit verbreitern,
			add ax,OFFSET GDT									; Offset addieren --> phys. Adresse
			mov DWORD PTR [pGDT+2],eax						; phys. Basisadresse der GDT in pGDT schreiben

; für Rücksprung in Real-Mode, Codesegment-Adresse unter RMODE_CS sichern
			mov ax,cs
			mov cs:RMODE_CS,ax
			
; Adresse der GDT setzen
			lgdt FWORD PTR [pGDT]
			
; Gate A20 aktivieren (Speicher bis 4GB)
EMPTY8042_1:
			in al,064h											; lese das Status-Port des 8042 (Keyboard Controller)
			test al,2											; AL und 2 werden UND-verknüpft und die entspr. Flags gesetzt
			jnz EMPTY8042_1									; Ausgabe des 8042: 0 = buffer empty, 2 = timeout
			
			mov al,0D1h											; 8042 Befehl, um ins Output Port zu schreiben
			out 064h,al
			
EMPTY8042_2:
			in al,064h											; lese das Status-Port des 8042 (Keyboard Controller)
			test al,2											; AL und 2 werden UND-verknüpft und die entspr. Flags gesetzt
			jnz EMPTY8042_2									; Ausgabe des 8042: 0 = buffer empty, 2 = timeout
			
			mov al,0DFh											; Daten fürs Output Port
			out 060h,al
			
EMPTY8042_3:
			in al,064h											; lese das Status-Port des 8042 (Keyboard Controller)
			test al,2											; AL und 2 werden UND-verknüpft und die entspr. Flags gesetzt
			jnz EMPTY8042_3									; Ausgabe des 8042: 0 = buffer empty, 2 = timeout

; keine Hardware Interrupts, da keine IDT definiert wurde
			cli
			
; Protected Mode aktivieren
			mov eax,cr0
			or eax,1
			mov cr0,eax

; "FAR-JMP" zu Selektor:Offset (entspr. jmp 8:ProtectedMode)
			db 0eah
			dw OFFSET ProtectedMode
			dw 8

ProtectedMode:
; Selektor für Video RAM zur direkten Stringausgabe nach ES
			mov ax,16
			mov es,ax

; Selektor für Datensegment nach DS
			mov ax,24
			mov ds,ax

; Stringausgabe
			mov di,(80*2)*24+(80-StrLength)*2				; Text am linken, unteren Bildschirmrand ausgeben
			mov si,OFFSET String
			mov cx,StrLength
			
SHOW:		mov al,[si]
			stosb					; überträgt den Inhalt von AL in eine Zeichenkette adressiert durch ES:DI und inkrementiert DI
			inc di												; Attribut Byte im Video RAM auslassen
			inc si
			loop SHOW											; Anzahl der Schleifendurchläufe in CX

; 386 Datensegment laden			
			mov ax,32											; 16 MB großes 386 Datensegment laden --> Gate A20 MUSS aktiviert sein
			mov es,ax
			
			mov eax,0
			xor edi,edi
			mov ecx,100h
			rep stosd											; schreibt 1024 Byte in dieses Datensegment
																	; falls Gate A20 deaktiviert, werden die DOS Interrupt-Handler überschr.

; benutzte Selektorregister ES und DS mit Datensegment der Länge FFFFh laden
; NUR SO KANN PROBLEMLOS IN DEN REAL-MODE ZURÜCKGESCHALTEN WERDEN!
			mov ax,24
			mov ds,ax
			mov es,ax

; zurück in den Real-Mode
			mov eax,cr0
			and eax,not 1
			mov cr0,eax

; "FAR-JMP" in den Real-Mode
			db 0eah
			dw OFFSET RealMode
RMODE_CS	dw ?

RealMode:
			sti
			
; Gate A20 deaktivieren	(Speicher bis 1 MB)
EMPTY8042_4:
			in al,064h											; lese das Status-Port des 8042 (Keyboard Controller)
			test al,2											; AL und 2 werden UND-verknüpft und die entspr. Flags gesetzt
			jnz EMPTY8042_4									; Ausgabe des 8042: 0 = buffer empty, 2 = timeout
			
			mov al,0D1h											; 8042 Befehl, um ins Output Port zu schreiben
			out 064h,al
			
EMPTY8042_5:
			in al,064h											; lese das Status-Port des 8042 (Keyboard Controller)
			test al,2											; AL und 2 werden UND-verknüpft und die entspr. Flags gesetzt
			jnz EMPTY8042_5									; Ausgabe des 8042: 0 = buffer empty, 2 = timeout
			
			mov al,0DDh											; Daten fürs Output Port
			out 060h,al
			
EMPTY8042_6:
			in al,064h											; lese das Status-Port des 8042 (Keyboard Controller)
			test al,2											; AL und 2 werden UND-verknüpft und die entspr. Flags gesetzt
			jnz EMPTY8042_6									; Ausgabe des 8042: 0 = buffer empty, 2 = timeout
			
			mov ax,4C00h
			int 21h
code		ends
			end START