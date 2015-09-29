;---------------------------------------------------------------------------
; PM_02.ASM (vgl. Beispiel: PM_01.ASM)
;
; zusätzlich: ANLEGEN EINER IDT MIT DESKRIPTOREN AUF EXCEPTIONS 0..17
;
; Versuch: Laden eines ungültigen Selektors in ein Selektorregister
; => löst Exception aus
;
; Das Programm funktioniert nicht unter Windows oder einem Expanded Memory
; Manager! Beide Programme laufen bereits im Protected Mode, der die
; Ausführung der hier benötigten (Betriebssystem-) Befehle verhindert.
;
; Erste Version:    08.11.96
; Letze Änderungen: 18.11.96
;                   19.03.97 - Interrupts ausschalten vor Manipulation der IDT
;                            - unnötigen Kommentar entfernt
;
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;---------------------------------------------------------------------------

    ; 386 Prozessor und Befehle für Protected Mode assemblieren
                .386p

;-- Stack ------------------------------------------------------------------
    STACKSIZE   equ 2000h
    stack       segment stack
                db STACKSIZE dup (?)
    stack       ends

;-- Konstante --------------------------------------------------------------
    b           equ byte ptr
    w           equ word ptr
    d           equ dword ptr
    o           equ offset
    BELL        equ 7

;-- Strukturen -------------------------------------------------------------
    GDTSTRUC    struc
        limit   dw ?                                    ; Größe der GDT
        base    dd ?                                    ; Basisadresse der GDT
    GDTSTRUC    ends

;-- Makros -----------------------------------------------------------------
    ; legt eine Struktur für einen Deskriptor an
    makeDESKRIPTOR macro LIMIT, BASE_015, BASE_1623, ACCESS, ATTR, BASE_2431
                dw LIMIT
                dw BASE_015
                db BASE_1623
                db ACCESS
                db ATTR
                db BASE_2431
                endm

    ; wandelt übergebenes SEGMENT:OFFSET Paar in 24 Bit Adresse um und
    ; trägt die ermittelte Adresse als Basisadresse in den Deskriptor N
    ; der GDT ein
    baseDESKRIPTOR macro N, SEGMENT, OFFSET
                mov ax,SEGMENT
                and eax,0FFFFh
                shl eax,4
                add ax,OFFSET

                mov w [GDT+8*N+2],ax
                shr eax,16
                mov b [GDT+8*N+4],al
                endm

    ; legt eine Struktur für ein Interrupt-Gate an
    makeIDT     macro OFFSET, SELEKTOR, ATTR
                dw OFFSET
                dw SELEKTOR
                db 0
                db ATTR
                dw 0
                endm

;-- Daten ------------------------------------------------------------------
    daten       segment USE16
                gdt_adr GDTSTRUC ?
                idt_adr GDTSTRUC ?

    ; GLOBALE DESKRIPTOR TABELLE
    GDT         label byte
                makeDESKRIPTOR 0,0,0,0,0,0                  ; wird grundsätzlich nicht benutzt
                makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0     ; Codesegment
                makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0     ; Datensegment (für Rückehr in Realmode)
                makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0     ; Stacksegment
                makeDESKRIPTOR 4000,8000h,0Bh,10010010b,0,0 ; für VRAM (0b8000h)
    GDTSIZE     equ $-GDT

    ; Selektoren für obengenannte Deskriptoren
    sNIL        equ 0000000000000000b
    sCODE       equ 0000000000001000b
    sDATA       equ 0000000000010000b
    sSTACK      equ 0000000000011000b
    sSCREEN     equ 0000000000100000b

    ; INTERRUPT DESKRIPTOR TABELLE
    IDT         label byte
                makeIDT IRQ_00, sCODE, 10001110b
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
                makeIDT 0,0,0
                makeIDT IRQ_16, sCODE, 10001110b
                makeIDT IRQ_17, sCODE, 10001110b
    IDTSIZE     equ $-IDT            

    ; für Rückkehr in Realmode
    saveSS      dw ?
    saveDS      dw ?

    ; für Ausgabe der allgemeinen Register
    r_eax       dd ?
    r_ebx       dd ?
    r_ecx       dd ?
    r_edx       dd ?
    r_esi       dd ?
    r_edi       dd ?
    r_esp       dd ?
    r_ebp       dd ?
    r_eip       dd ?
    r_cs        dw ?
    r_ds        dw ?
    r_es        dw ?
    r_fs        dw ?
    r_gs        dw ?
    r_ss        dw ?

    exception   db ?

    ; eine ASCII Beschreibung der auftretenden Exceptions
    exception_names label byte
                db "(Fehler bei Division)               "
    eintrag_laenge equ $-exception_names
                db "(Debugger-Interrupt)                "
                db "(NMI)                               " 
                db "(Breakpoint)                        "
                db "(Überlauf (INTO))                   "
                db "(Arraygrenzen überschritten (BOUND))"
                db "(ungültiger Opcode)                 "
                db "(Coprozessor nicht installiert)     "
                db "(Doppelfehler)                      "
                db "(Segment-überlauf des Coprozessors) "
                db "(TSS ungültig)                      "
                db "(Segment nicht im Hauptspeicher)    "
                db "(Stack-überlauf)                    "
                db "(allgemeine Schutzverletzung)       "
                db "(Paging Ausnahmezustand)            "
                db "(reserviert)                        "
                db "(Coprozessor-Fehler)                "
                db "(Ausrichtungfehler)                 "

    ; Exception Text, wird nach der Rückschaltung in den rmode über
    ; Funktion 09h des Int 21h ausgegeben
    txt         db BELL, "Exception "
    NR          db "00 "
    oNAME       db eintrag_laenge dup (' ')
                db 13,10,10
                db "cs = "
    oCS         db "0000     "
                db "eip= "
    oEIP        db "00000000 ", 13, 10
                db "ss = "
    oSS         db "0000     "
                db "esp= "
    oESP        db "00000000 ", 13, 10
                db "eax= "
    oEAX        db "00000000 "
                db "ebx= "
    oEBX        db "00000000 "
                db "ecx= "
    oECX        db "00000000 "
                db "edx= "
    oEDX        db "00000000 ", 13, 10
                db "esi= "
    oESI        db "00000000 "
                db "edi= "
    oEDI        db "00000000 "
                db "ebp= "
    oEBP        db "00000000 ", 13, 10
                db "ds = "
    oDS         db "0000     "
                db "es = "
    oES         db "0000     "
                db "fs = "
    oFS         db "0000     "
                db "gs = "
    oGS         db "0000     ", 13,10
                db "$"
    nul         dd ?
    daten       ends

;-- Codesegment ------------------------------------------------------------
    code        segment use16
                assume  cs:code, ds:daten, ss:stack
    include tool.inc

    start:
                mov ax,daten
                mov ds,ax
                mov saveDS,ax

                mov ax,ss
                mov saveSS,ax

    ; Basisadressen Init
                baseDESKRIPTOR 1, cs, 0
                baseDESKRIPTOR 2, ds, 0
                baseDESKRIPTOR 3, ss, 0

    ; Adresse der GDT setzen
                mov ax,GDTSIZE
                mov [gdt_adr.limit],ax

                mov ax,seg GDT
                and eax,0FFFFh
                shl eax,4
                add ax,offset GDT
                mov [gdt_adr.base],eax
                LGDT gdt_adr

    ; Interrupts aus, IDT wird verändert
                cli

    ; Adresse der IDT setzen
                mov ax,IDTSIZE
                mov [idt_adr.limit],ax

                mov ax,seg IDT
                and eax,0FFFFh
                shl eax,4
                add ax,offset IDT
                mov [idt_adr.base],eax
                LIDT idt_adr

    ; für Rücksprung, Codesegment-Adresse unter rmode_cs sichern
                mov ax,cs
                mov cs:rmode_cs,ax
                mov cs:rmode_cs2,ax

    ; Protected Mode aktivieren
                mov eax,cr0
                or eax,1
                mov cr0,eax

    ; "FAR-JMP" zu Selektor:Offset (entspricht jmp sCODE:pmode)
                db 0eah
                dw offset pmode
                dw sCODE

    pmode:
    ; Selektor für Stack Segment
                mov ax,sSTACK
                mov ss,ax
                mov esp,STACKSIZE

    ; Register vorbereiten -> Schattenregister init
                mov ax,sDATA
                mov ds,ax
                mov es,ax
                mov fs,ax
                mov gs,ax

;----------------------------------------------------------------------------
; PMODE
;----------------------------------------------------------------------------

    ; Versuch: "sinnlosen" Selektor zu laden, endet in Exception 0Dh
                mov ax,1234h
                mov ds,ax

;----------------------------------------------------------------------------
; ZURÜCK ZUM REALMODE
;----------------------------------------------------------------------------
    back_to_rmode:
                cli

                mov ax,sDATA
                mov ds,ax
                mov es,ax
                mov fs,ax
                mov gs,ax
                mov ss,ax

                mov eax,cr0
                and eax,NOT 1
                mov cr0,eax

                db 0eah
                dw offset rmode
    rmode_cs    dw ?

    rmode:
    ; Interrupt-Deskriptortabelle wieder auf Realmode-Standard
                mov [idt_adr.base],0
                mov [idt_adr.limit],03ffh
                LIDT idt_adr

    ; Stack Segmentregister auf alten Wert
                mov ax,saveSS
                mov ss,ax
                mov sp,01000h

    ; zurück zu DOS
                sti
                mov ax,4c00h
                int 21h

;---------------------------------------------------------------------------
; Exception Handler für Exceptions 0..17
;---------------------------------------------------------------------------
    IRQ_00:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,0
                jmp quit_all

    IRQ_01:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,1
                jmp quit_all

    IRQ_02:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,2
                jmp quit_all

    IRQ_03:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,3
                jmp quit_all

    IRQ_04:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,4
                jmp quit_all

    IRQ_05:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,5
                jmp quit_all

    IRQ_06:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,6
                jmp quit_all

    IRQ_07:
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,7
                jmp quit_all

    IRQ_08:
                jmp back_to_rmode

    IRQ_09:
                pop nul
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,9
                jmp quit_all

    IRQ_10:
                pop nul
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,10
                jmp quit_all

    IRQ_11:
                pop nul
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,11
                jmp quit_all

    IRQ_12:
    ; Stack-Fault, keine Behandlung wie obige Interrupt-Routinen möglich
    ; (der Stack ist außer Kontrolle) daher nur zurück in den Realmode ...
                jmp back_to_rmode

    IRQ_13:
                pop nul
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,13
                jmp quit_all

    IRQ_14:
                pop nul
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,14
                jmp quit_all

    IRQ_16:
                pop nul
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,16
                jmp quit_all

    IRQ_17:
                pop nul
                push esp ds eax
                mov ax,sDATA
                mov ds,ax
                pop eax
                mov r_eax,eax
                mov al,17
                jmp quit_all


;---------------------------------------------------------------------------
; PROGRAMM ABBRUCH DURCH EXCEPTION, RÜCKKEHR IN DEN REALMODE UND
; AUSGABE ALLER REGISTER
;---------------------------------------------------------------------------
    quit_all:
                mov exception, al
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

    ; ZURÜCK ZUM REALMODE
                cli

                mov ax,sDATA
                mov ds,ax
                mov es,ax
                mov fs,ax
                mov gs,ax
                mov ss,ax

                mov eax,cr0
                and eax,NOT 1
                mov cr0,eax

                db 0eah
                dw offset rmode2
    rmode_cs2   dw ?

    rmode2:
    ; Interrupt-Deskriptortabelle wieder auf Realmode-Standard
                mov [idt_adr.base],0
                mov [idt_adr.limit],03ffh
                LIDT idt_adr

    ; Stack Segmentregister auf alten Wert
                mov ax,saveSS
                mov ss,ax
                mov sp,01000h

                mov ax,saveDS
                mov ds,ax
                mov es,ax

    ; Umwandlung der Registerwerte in hexadez. ASCII Werte und ...
                mov al,exception
                mov ah,0
                mov di,o NR
                call ShowHexByte

                mov eax,r_eax
                mov di,o oEAX
                call ShowHexDword

                mov eax,r_ebx
                mov di,o oEBX
                call ShowHexDword

                mov eax,r_ecx
                mov di,o oECX
                call ShowHexDword

                mov eax,r_edx
                mov di,o oEDX
                call ShowHexDword

                mov eax,r_esi
                mov di,o oESI
                call ShowHexDword

                mov eax,r_edi
                mov di,o oEDI
                call ShowHexDword

                mov eax,r_ebp
                mov di,o oEBP
                call ShowHexDword

                mov eax,r_esp
                mov di,o oESP
                call ShowHexDword

                mov eax,r_eip
                mov di,o oEIP
                call ShowHexDword

                mov ax,r_cs
                mov di,o oCS
                call ShowHexWord

                mov ax,r_ds
                mov di,o oDS
                call ShowHexWord

                mov ax,r_es
                mov di,o oES
                call ShowHexWord

                mov ax,r_fs
                mov di,o oFS
                call ShowHexWord

                mov ax,r_gs
                mov di,o oGS
                call ShowHexWord

                mov ax,r_ss
                mov di,o oSS
                call ShowHexWord

                mov al,exception
                and ax,0FFh
                mov bl,eintrag_laenge
                mul bl
                add ax,o exception_names
                mov si,ax
                mov di,o oNAME
                mov cx,eintrag_laenge
                rep movsb

    ; Ausgabe: EXCEPTION ...
                mov dx,offset txt
                mov ah,09
                int 21h

    ; zurück zu DOS
                mov ax,4c01h
                int 21h
    code        ends
                end start

