; Protected Mode Startup-Code für pm_03.c !
; (c) Copyright 1997 by Jens Hohmuth. All rights reserved.

                    .386p

;-- Allgemeines -------------------------------------------------------------
    ; _main ist die C-Funktion, die aus dem Prot. Mode heraus gerufen wird
    extrn           _main:near

    ; _DS_START hält die (lineare) Startadresse des Datensegmentes fest und
    ; dient der Umrechnung von linearen in relative Adressen
    public          _DS_START

;-- Konstante ---------------------------------------------------------------
    b               equ byte ptr
    w               equ word ptr
    d               equ dword ptr
    o               equ offset

    sNIL            equ 0
    sCODE16         equ 08h
    sDATA16         equ 10h
    sTEXT           equ 18h
    sDATA           equ 20h
    sBSS            equ 28h
    sSTACK          equ 30h

;-- Strukturen --------------------------------------------------------------
    GDTSTRUC        struc
        limit       dw ?
        base        dd ?
    GDTSTRUC        ends

;-- Segmentdefinitionen -----------------------------------------------------
    _TEXT           segment para public use32 'CODE'
    _TEXT           ends
    _DATA           segment para public use32 'DATA'
    _DATA           ends
    _BSS            segment para public use32 'BSS'
    _BSS            ends
    _STACK          segment stack
                    db 2000h dup (?)
    _STACK          ends

;-- 16Bit Datensegment ------------------------------------------------------
    _DATA16         segment dword public use16
        gdt_adr     GDTSTRUC ?

        GDT         label byte
        des_dummy   db 8 dup (0)

        des_code16  dw 0FFFFh
                    dw ?                 ; wird zur Laufzeit eingetragen
                    db ?                 ; wird zur Laufzeit eingetragen
                    db 10011010b
                    dw 0

        des_data16  dw 0FFFFh
                    dw ?                 ; wird zur Laufzeit eingetragen
                    db ?                 ; wird zur Laufzeit eingetragen
                    db 10010010b
                    dw 0

        des_text    dw 0FFFFh
                    dw ?                 ; wird zur Laufzeit eingetragen
                    db ?                 ; wird zur Laufzeit eingetragen
                    db 10011010b
                    db 01000000b
                    db 0

        des_data    dw 0FFFFh
                    dw ?                 ; wird zur Laufzeit eingetragen
                    db ?                 ; wird zur Laufzeit eingetragen
                    db 10010010b
                    db 11001111b
                    db 0

        des_bss     dw 0FFFFh
                    dw ?                 ; wird zur Laufzeit eingetragen
                    db ?                 ; wird zur Laufzeit eingetragen
                    db 10010010b
                    db 01000000b
                    db 0

        des_stack   dw 0FFFFh
                    dw ?                 ; wird zur Laufzeit eingetragen
                    db ?                 ; wird zur Laufzeit eingetragen
                    db 10010010b
                    db 01000000b
                    db 0
        GDTSIZE     equ $-GDT
    _DATA16         ends

;-- 16Bit Codesegment - eigentlicher Startup Code ---------------------------
    _TEXT16         segment para public use16
                    assume  cs:_TEXT16, ss:_STACK, ds:_DATA16

    start:
                    mov ax,_DATA16
                    mov ds,ax

    ;------------------------------------------------------------------------
    ; PMODE INIT !
    ;------------------------------------------------------------------------

    ; private (16Bit) Segmente
                    mov ax,cs
                    and eax,0FFFFh
                    shl eax,4
                    add eax,offset start

                    mov w [des_code16+2],ax
                    shr eax,16
                    mov b [des_code16+4],al
    ;
                    mov ax,ds
                    and eax,0FFFFh
                    shl eax,4

                    mov w [des_data16+2],ax
                    shr eax,16
                    mov b [des_data16+4],al

    ; C (32Bit) Segmente
                    mov ax,_TEXT
                    and eax,0FFFFh
                    shl eax,4

                    mov w [des_text+2],ax
                    shr eax,16
                    mov b [des_text+4],al
    ;
                    mov ax,_DATA
                    and eax,0FFFFh
                    shl eax,4

                    assume ds:_DATA
                    mov bx,_DATA
                    mov es,bx
                    mov es:[_DS_START],eax
                    assume ds:_DATA16

                    mov w [des_data+2],ax
                    shr eax,16
                    mov b [des_data+4],al
    ;
                    mov ax,_BSS
                    and eax,0FFFFh
                    shl eax,4

                    mov w [des_bss+2],ax
                    shr eax,16
                    mov b [des_bss+4],al
    ;
                    mov ax,_STACK
                    and eax,0FFFFh
                    shl eax,4

                    mov w [des_stack+2],ax
                    shr eax,16
                    mov b [des_stack+4],al

    ; Adresse und Größe der GDT in der GDTSTRUCT setzen
                    mov ax,GDTSIZE
                    mov [gdt_adr.limit],ax

                    mov ax,seg GDT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,offset GDT
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

    ; Sprung in das 32Bit Segment sTEXT und zur Offsetadresse start32
                    db 0eah
                    dw offset start32
                    dw sTEXT

    ;------------------------------------------------------------------------
    ; PMODE DEINIT !
    ;------------------------------------------------------------------------
    ; start32 kehrt (später, nach dem Ende von _main) hierher zurück

    pmode_deinit:
                    mov ax,sDATA16
                    mov ss,ax
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax

    ; zurück in den Realmode
                    mov eax,cr0
                    and eax,NOT 1
                    mov cr0,eax

    ; "FAR-JMP" in den Realmode
                    db 0eah
                    dw offset rmode
    rmode_cs        dw ?

    rmode:
                    mov ax,_STACK
                    mov ss,ax
                    mov esp,1000h

                    mov ax,_DATA16
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax

                    sti
                    mov eax,4c00h
                    int 21h
    _TEXT16         ends

;-- 32Bit Codesegment, im Protected Mode ausgeführt -------------------------

    _TEXT           segment para public use32 'CODE'
                    assume cs:_TEXT

    start32:

    ; Selektor-Register Initialisierung
                    mov ax,sSTACK
                    mov ss,ax
                    mov esp,1000h

                    mov ax,sDATA
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax

    ;------------------------------------------------------------------------
    ; CALL C MAIN FUNCTION
    ;------------------------------------------------------------------------
                    xor eax,eax
                    xor ebx,ebx
                    xor ecx,ecx
                    xor edx,edx
                    xor esi,esi
                    xor edi,edi
                    call _main

    ; zurück ins 16Bit Segment, um den Realmode wieder zu aktivieren
                    db 0eah
                    dd offset pmode_deinit
                    dw sCODE16
    _TEXT           ends

;-- 32Bit Datensegment ------------------------------------------------------
    _DATA           segment para public use32 'DATA'
        _DS_START   dd ?
    _DATA           ends

                    end start
