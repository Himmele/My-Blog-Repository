;---------------------------------------------------------------------------
; PM_01.ASM
;
; Minimales Testprogramm um den Prozessor in den Protected Mode zu schalten.
; Ein 80386-Prozessor wird vorausgesetzt!
;
; Das Programm funktioniert nicht unter Windows oder einem Expanded Memory
; Manager! Beide Programme laufen bereits im Protected Mode, der die
; Ausführung der hier benötigten (Betriebssystem-) Befehle verhindert.
;
; Es werden die folgenden 5 Schritte durchgeführt:
;
; 1) anlegen einer GDT, sowie Laden des GDTR-Registers mit der Adresse der GDT
; 2) Bit 0 (PE-Bit) im Register CR0 setzen, um Protected Mode zu aktivieren
; 3) durchführen eines "FAR-JMP" um die Warteschlange zu leeren und das
;    Selektoregister CS (sowie das Deskriptorschattenregister) mit einem
;    gültigen Wert zu initialisieren
; 4) direkte Ausgabe eines Stringes in den Bildschirmspeicher
; 5) Rückehr in den Realmode
;
; Erste Version:    29.10.96
; Letze Änderungen: 05.11.96
;                   19.03.97 - Kommentar 3) siehe oben
;                            - Programm nutzt jetzt DS als Quelle für
;                              Stringausgabe (früher CS)
;
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;---------------------------------------------------------------------------

    ; 386 Prozessor und Befehle für Protected Mode assemblieren
                .386p

;-- Stack ------------------------------------------------------------------
    stack       segment stack
                dw 100h dup (?)
    stack       ends

;-- Konstante --------------------------------------------------------------
    b           equ byte ptr
    w           equ word ptr
    d           equ dword ptr

;-- Strukturen -------------------------------------------------------------
    GDTSTRUC    struc
        limit   dw ?                                    ; Größe der GDT
        base    dd ?                                    ; Basisadresse der GDT
    GDTSTRUC    ends

;-- Daten ------------------------------------------------------------------
    daten       segment USE16
    gdt_adr     GDTSTRUC ?
    ;
    ; die GLOBALE DESKRIPTOR TABELLE
    ;
    GDT         label byte

    ; Deskriptor 0, wird nicht benutzt
       des_dummy   db 8 dup (0)

    ; Deskriptor 1, CODESEGMENT DESKRIPTOR
       des_code    dw 0FFFFh
                   dw ?                 ; wird zur Laufzeit eingetragen
                   db ?                 ; wird zur Laufzeit eingetragen
                   db 10011010b
                   db 00000000b
                   db 0

    ; Deskriptor 2, VIDEO RAM: 0B800h
       des_vram    dw 4000
                   dw 8000h
                   db 0Bh
                   db 10010010b
                   db 00000000b
                   db 0

    ; Deskriptor 3, entspricht akt. REALMODE DATENSEGMENT
       des_data    dw 0FFFFh
                   dw ?                 ; wird zur Laufzeit eingetragen
                   db ?                 ; wird zur Laufzeit eingetragen
                   db 10010010b
                   db 00000000b
                   db 0

    GDTSIZE     equ $-GDT

    string      db "Hallo Welt vom Protected Modus !!!"
    string_len  equ $-string
    daten       ends

;-- Codesegment -------------------------------------------------------------
    code        segment use16
                assume  cs:code, ds:daten, ss:stack

    start:
                mov ax,daten
                mov ds,ax

    ; Codesegment Basisadresse in Deskriptor 1 eintragen
                mov ax,cs
                and eax,0FFFFh
                shl eax,4
                add ax,offset start

                mov w [des_code+2],ax
                shr eax,16
                mov b [des_code+4],al

    ; Datensegment Basisadresse in Deskriptor 3 eintragen
                mov ax,ds
                and eax,0FFFFh
                shl eax,4

                mov w [des_data+2],ax
                shr eax,16
                mov b [des_data+4],al

    ; Adresse und Größe der GDT in der GDTSTRUCT setzen
                mov ax,GDTSIZE
                mov [gdt_adr.limit],ax

                mov ax,seg GDT
                and eax,0FFFFh
                shl eax,4
                add ax,offset GDT
                mov [gdt_adr.base],eax

    ; für Rücksprung, Codesegment-Adresse unter rmode_cs sichern
                mov ax,cs
                mov cs:rmode_cs,ax

    ; Adresse der GDT setzen
                LGDT gdt_adr

    ; Keine Hardware - Interrupts zulassen, da keine IDT definiert wurde :)
                cli

    ; Protected Mode aktivieren
                mov eax,cr0
                or eax,1
                mov cr0,eax

    ; "FAR-JMP" zu Selektor:Offset (entspricht jmp 8:pmode)
                db 0eah
                dw offset pmode
                dw 8

    pmode:
    ; Selektor für Video Ram zur direkten Stringausgabe nach ES
                mov ax,16
                mov es,ax

    ; Selektor für Datensegment nach DS
                mov ax,24
                mov ds,ax

    ; Stringausgabe
                mov di,(80*2)*24+(80-string_len)*2

                mov si,offset string
                mov cx,string_len

    showtext:
                mov al,[si]
                stosb
                inc di
                inc si
                loop showtext

    ; benutzte Selektoregister ES und DS mit Datensegment der Länge FFFFh laden.
    ; NUR SO KANN PROBLEMLOS IN DEN REALMODE ZURÜCKGESCHALTET WERDEN !
                mov ax,24
                mov ds,ax
                mov es,ax

    ; und gleich wieder zurück in den Realmode
                mov eax,cr0
                and eax,NOT 1
                mov cr0,eax

    ; "FAR-JMP" in den Realmode
                db 0eah
                dw offset rmode
    rmode_cs    dw ?

    rmode:
                sti
                mov ax,4c00h
                int 21h
    code        ends
                end start

