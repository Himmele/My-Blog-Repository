;----------------------------------------------------------------------------
; PM_03.ASM (vgl. PM_01.ASM / PM_02.ASM)
; zusätzlich: Aktivieren des Paging-Mechanismus
;----------------------------------------------------------------------------
;
; Das Programm funktioniert nicht unter Windows oder einem Expanded Memory
; Manager! Beide Programme laufen bereits im Protected Mode, der die
; Ausführung der hier benötigten (Betriebssystem-) Befehle verhindert.
;
; Erste Version:    27.11.96
; Letze Änderungen: 03.12.96
;                   19.03.97 - EDI auf 0 initialisieren, bei "stosd" !!!
;                   28.03.97 - keine Exception Unterstützung mehr
;                              (einfacherer und kürzerer Quellcode)
;                            - Pagetables werden im Datensegment vereinbart
;                              (kein DOS-Heap mehr notwendig)
;
; (c) Copyright 1997 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;----------------------------------------------------------------------------

    ; 386 Prozessor und Befehle für Protected Mode assemblieren
                .386p

;-- Stack -------------------------------------------------------------------
    STACKSIZE   equ 2000h
    stack       segment stack
                db STACKSIZE dup (?)
    stack       ends

;-- Konstante ---------------------------------------------------------------
    b           equ byte ptr
    w           equ word ptr
    d           equ dword ptr
    o           equ offset

;-- Strukturen --------------------------------------------------------------
    GDTSTRUC    struc
        limit   dw ?                                    ; Größe der GDT
        base    dd ?                                    ; Basisadresse der GDT
    GDTSTRUC    ends

;-- Makros ------------------------------------------------------------------
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

    ; legt eine 4 Byte lange Struktur an, die als Pagetable Eintrag
    ; interpretiert wird, erwartet das Attribut-Byte des Eintrags (ATTRIBUT)
    ; sowie die phys. Speicherseitennummer (PAGEFRAME)
    ;
    ; !! Es ist wichtig die Intel Konvention zu beachten, LOW BYTE an der
    ;    niederwertigen ADRESSE !!!
    ;
    makePAGE    macro PAGEFRAME, ATTRIBUT
                db ATTRIBUT
                db (PAGEFRAME AND 0Fh) SHL 4 + 0000b
                dw PAGEFRAME SHR 4
                endm

;-- Daten ------------------------------------------------------------------
    daten       segment USE16
                gdt_adr GDTSTRUC ?

    ; GLOBALE DESKRIPTOR TABELLE
    GDT         label byte
                ; wird grundsätzlich nicht benutzt
                makeDESKRIPTOR 0,0,0,0,0,0

                ; Codesegment
                makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0

                ; Datensegment (für Rückehr in Realmode)
                makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0

                ; Stacksegment
                makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0

                ; für VRAM (0a0000h)
                makeDESKRIPTOR 0FFFFh,0000h,0Ah,10010010b,0,0

                ; für VRAM (0b0000h)
                makeDESKRIPTOR 0FFFFh,0000h,0Bh,10010010b,0,0
    GDTSIZE     equ $-GDT

    ; Selektoren für obengenannte Deskriptoren
    sNIL        equ 0000000000000000b
    sCODE       equ 0000000000001000b
    sDATA       equ 0000000000010000b
    sSTACK      equ 0000000000011000b
    sSCREEN     equ 0000000000100000b
    sSCREEN2    equ 0000000000101000b

;-- PAGETABLE ------------------------------------------------------------------
    ; Pagetable wird später an richtige (durch 4096 teilbare) Adresse
    ; kopiert ...
    page_table  label byte
                db 0A0h*4 dup (?)

    ; Bereich: 0A0000h..0AFFFFh -> 0A0000h..0AFFFFh
                makePAGE 0A0h, 1
                makePAGE 0A1h, 1
                makePAGE 0A2h, 1
                makePAGE 0A3h, 1
                makePAGE 0A4h, 1
                makePAGE 0A5h, 1
                makePAGE 0A6h, 1
                makePAGE 0A7h, 1
                makePAGE 0A8h, 1
                makePAGE 0A9h, 1
                makePAGE 0AAh, 1
                makePAGE 0ABh, 1
                makePAGE 0ACh, 1
                makePAGE 0ADh, 1
                makePAGE 0AEh, 1
                makePAGE 0AFh, 1

    ; Bereich: 0B0000h..0BFFFFh -> 0A0000h..0AFFFFh
    ; (mit Veränderungen bei ausgewählten Seiten)
                makePAGE 0A0h, 1
                makePAGE 0A1h, 1
                makePAGE 0A2h, 1
                makePAGE 0A3h, 1
                makePAGE 0A4h, 1
                makePAGE 0AFh, 1 
                makePAGE 0AEh, 1
                makePAGE 0ADh, 1
                makePAGE 0ACh, 1
                makePAGE 0ABh, 1
                makePAGE 0AAh, 1
                makePAGE 0A9h, 1
                makePAGE 0A8h, 1
                makePAGE 0A7h, 1
                makePAGE 0A6h, 1
                makePAGE 0A5h, 1

    ; ab hier wieder "normales" Mapping
    second_tab  label byte
                db (100h-0C0h)*4 dup (?)

    ; für Rückehr in den Realmode
    saveSS      dw ?
    saveDS      dw ?

    ; Pagetable Speicherplätze, werden zur Laufzeit an einer durch 4096
    ; teilbaren Adresse angepaßt
    pagedir     db 2000h dup (?)
    pagetab     db 2000h dup (?)
    p_pagedir   dw ?
    p_pagetab   dw ?

    daten       ends

;-- Codesegment ------------------------------------------------------------
    code        segment use16
                assume  cs:code, ds:daten, ss:stack

    ; Farbpalettendaten und Routine zum Setzen dieser Palette
    ; (blauer Farbverlauf)
    include     setpal.inc

    ; PAGETABLES werden in einen Speicherblock kopiert, der von
    ; DOS angefordert wird. Die Adressen dieser Speicherblöcke werden
    ; hier gespeichert ...
    ;
    ; PROBLEM: Pagetables müssen immer an einer durch 4096 teilbaren Adresse
    ;          beginnen!
    ;          Es ist (nach meinem aktuellen Wissenstand, 29.11.1996)
    ;          nicht möglich eine solche Ausrichtung von Daten innerhalb
    ;          eines Assemblerprogr. zu erreichen (z.B. durch ALIGN 12)
    ;          Dieses Programm reserviert darum 8192 Byte je Pagetable und
    ;          sucht zur Laufzeit eine durch 4096 teilbare Adresse (in diesem
    ;          Speicherbereich) an den die Pagetable kopiert wird ...
    pagedir_adr_DOS dw ?                    ; Segmentadresse des DOS-Speicherbereichs
        pagedir_adr dw ?                    ; durch 4096 teilbare Adresse innerhalb des
                                            ; DOS-Speicherbereichs ...
    pagetab_adr_DOS dw ?
        pagetab_adr dw ?
    start:
                cld
                mov ax,daten
                mov ds,ax
                mov saveDS,ax

    ; Ermittlung einer durch 4096 teilbaren Adresse innerhalb des DATEN-
    ; Segmentes (reservierter Speicherblock: pagedir)
                mov ax,ds
                and eax,0ffffh
                shl eax,4
                add ax,o pagedir                    ; eax= phys. Adr.
                and eax,0FFFFF000h
                add eax,01000h                      ; nächstes Vielfache von 4096
                shr eax,4                           ; in Segmentadresse umwandeln
                mov p_pagedir,ax

    ; Ermittlung einer durch 4096 teilbaren Adresse innerhalb des DATEN-
    ; Segmentes (reservierter Speicherblock: pagetab)
                mov ax,ds
                and eax,0ffffh
                shl eax,4
                add ax,o pagetab
                and eax,0FFFFF000h
                add eax,01000h
                shr eax,4
                mov p_pagetab,ax

    ; pagedir (p_pagedir) vorbereiten:
    ; Adresse der Page-Table im Page-Directory eintragen
                mov di,p_pagedir
                mov es,di
                xor edi,edi

                movzx eax,p_pagetab
                shl eax,4                           ; Pagenummer in Bits (31..12)
                add eax,1                           ; Present Bit= 1
                stosd                               ; Eintragen in pagedir

    ; keine Unterschiede zwischen linear / physischen Adressen (0..09FFFFh)
    ; page_table
                mov di,seg page_table       
                mov es,di
                xor edi,edi
                mov di,o page_table                 ; es:edi -> 1. Eintrag in Pagetable

                mov ebx,0
                mov ecx,0A0h

    erster_teil:
                mov eax,ebx
                shl eax,12                          ; Pagenummer in Bits (31..12)
                add eax,1                           ; Present Bit= 1
                stosd

                inc ebx
                loop erster_teil

    ; keine Unterschiede zwischen linear / physischen Adressen (0C0000h..0FFFFFh)
                mov edi,0C0h*4
                add di,o page_table
                mov ebx,0C0h
                mov ecx,0100h-0C0h

    zweiter_teil:
                mov eax,ebx
                shl eax,12                          ; Pagenummer in Bits (31..12)
                add eax,1                           ; Present Bit= 1
                stosd

                inc ebx
                loop zweiter_teil

    ; pagetable kopieren (page_table -> p_pagetab)
                mov si,o page_table
                mov di,p_pagetab
                mov es,di
                xor edi,edi

                mov ecx,4096
                rep movsb

    ; Stacksegment sichern
                mov ax,ss
                mov saveSS,ax

    ; Grafik ein
                mov ax,13h
                int 10h
                call setpal

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

    ; alle Interrupt aus (nicht alle Interrupts können behandelt werden, es
    ; wurden keine Handler für Hardware-Interrupts bereitgestellt)
                cli
                in al,70h
                or al,80h
                out 70h,al

    ; für Rücksprung, Codesegment-Adresse unter <rmode_cs> sichern
                mov ax,cs
                mov cs:rmode_cs,ax

    ; Protected Mode aktivieren
                mov eax,cr0
                or eax,1
                mov cr0,eax

    ; "FAR-JMP" zu Selektor:offset (entspricht jmp sCODE:pmode)
                db 0eah
                dw offset pmode
                dw sCODE

    pmode:
    ; Selektor für Stack Segment setzen
                mov ax,sSTACK
                mov ss,ax
                mov esp,STACKSIZE

    ; Selektor Register vorbereiten
                mov ax,sDATA
                mov ds,ax
                mov es,ax
                mov fs,ax
                mov gs,ax

;----------------------------------------------------------------------------
; PMODE 
;----------------------------------------------------------------------------
    ; Register CR3 initialisieren, Adresse des Pagetables eintragen
                mov ax,p_pagedir
                and eax,0ffffh
                shl eax,4
                mov cr3,eax

    ; PG Bit (Bit 31) setzen um Paging zu aktivieren
                mov ebx,cr0
                or ebx,80000000h
                mov cr0,ebx

    ; "Muster" auf VGA-Screen kopieren
                mov ax,sSCREEN
                mov es,ax
                xor edi,edi                         ; es:edi -> 0A0000h:0

                mov edx,200                         ; 200 Zeilen
                mov eax,0                           ; Farbwert je Zeile

    again:
                mov ecx,320 / 4                     ; Zeile mit Farbe <ax> füllen
                rep stosd

                add eax,01010101h                   ; nächster Farbindex
                dec edx                             ; schon alle Zeilen ?
                jnz again                           ; nein, dann weiter

    ; wartet auf <ESC> ...
    ; Es steht weder DOS noch eine BIOS-Routine zur Verfügung, um die
    ; Tastatur abzufragen, da die Interrupts abgeschaltet wurden.
    ; So wird auch der Tastatur-Irq 9 nicht ausgeführt und damit der
    ; Tastatur-Buffer nicht aktualisiert.
    ;
    ; Aus diesen Gründen wird direkt der Tastaturstatus abgefragt und
    ; solange gewartet bis die Taste <ESC> (Scancode 1) gedrückt wird.
    waitkey:
                in al,[60h]
                cmp al,1
                jne waitkey

    ; "Muster" nach 0b0000h kopieren,
    ; wurde ins VGA-Ram "gemappt", darausfolgt daß das Muster ebenfalls
    ; auf dem Bildschirm erscheint
                mov ax,sSCREEN2
                mov es,ax
                xor edi,edi                         ; es:edi -> 0B0000h:0

                mov edx,200                         ; 200 Zeilen
                mov eax,0                           ; Farbwert je Zeile

    again2:
                mov ecx,320 / 4                     ; Zeile mit Farbe <ax> füllen
                rep stosd

                add eax,01010101h                   ; nächster Farbindex
                dec edx                             ; schon alle Zeilen ?
                jnz again2                          ; nein, dann weiter

    ; Wartet darauf, daß die ESC-Taste "losgelassen" wird
    releasekey:
                in al,[60h]
                cmp al,81h
                jne releasekey

    ; Paging Aus ...
                mov eax,cr0
                and eax,7FFFFFFFh
                mov cr0,eax

                mov eax,0
                mov cr3,eax

;----------------------------------------------------------------------------
; ZURÜCK ZUM REALMODE
;----------------------------------------------------------------------------
    back_to_rmode:
                mov ax,sDATA
                mov ds,ax
                mov es,ax
                mov fs,ax
                mov gs,ax
                mov ss,ax

    ; Protected Mode aus ...
                mov eax,cr0
                and eax,NOT 1
                mov cr0,eax

                db 0eah
                dw offset rmode
    rmode_cs    dw ?

    rmode:
    ; Stack Segmentregister auf alten Wert
                mov ax,saveSS
                mov ss,ax
                mov sp,STACKSIZE

                in al,70h
                or al,not 80h
                out 70h,al
                sti

    ; auf Taste warten
                mov ah,0
                int 16h

    ; Textmode initialisieren
                mov ax,3
                int 10h

    ; zurück zu DOS
                mov ax,4c00h
                int 21h
    code        ends
                end start
