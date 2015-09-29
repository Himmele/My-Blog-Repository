; kooperatives Multitasking - Haupt-Task

; 386 Prozessor und Befehle für den Protected Mode assemblieren
                .386p

;---------------------------------------------------------------------------------------------------------------------------

 extrn          PM_Init:FAR							; wird in externem Modul definiert
 
;---------------------------------------------------------------------------------------------------------------------------
; Stack
stack           segment stack
                db 1000 dup (?)
stack           ends

;---------------------------------------------------------------------------------------------------------------------------
 data	          segment USE16 PUBLIC
       back     label word                      ; Rücksprung in Initialisierungs-Task
                dw 0
                dw 0
       Message  db "kooperatives Multitasking",0
 data	          ends

;---------------------------------------------------------------------------------------------------------------------------
                assume  cs:code, ds:data, ss:stack
code            segment USE16

START:
; Protected Mode mit Hilfe von PM_Init einschalten
                call PM_Init
                mov [back+2],ax                 ; PM_Init sichert Rücksprung Selektor zum Initialisierungs-Task in AX

; ab jetzt eigenständiger Task
                xor esi,esi
                mov si,OFFSET Message
                mov edi,(80*2)*24

OUTPUT:
                lodsb									; lädt ein Byte von DS:SI nach AL
                or al,al                        ; Byte = 0 (Textende) ?
                je QUIT

                stosb									; überträgt AL nach ES:DI (ES ist im TSS mit Video-RAM initialisiert)
                inc di
                jmp OUTPUT

; zurück zum Initialisierungs-Task (Sprung zum TSS-Selektor des Tasks)
QUIT:
                jmp DWORD PTR [back]				; lädt TSS-Selektor des Initialisierungs-Tasks -> Task-Switch
code            ends
                end START