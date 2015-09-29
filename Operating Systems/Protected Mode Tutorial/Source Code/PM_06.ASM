;---------------------------------------------------------------------------
; PM_06.ASM - Protected Mode Init / Deinit mit DPMI
;           - Ausgabe eines Textes mit DOS-Fkt. 09 (im Protected Mode !!!)
;           - erfordert DPMI-Host (z.B. Windows)
;---------------------------------------------------------------------------
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:    06.01.97
; Letze Änderungen: 07.01.97
;---------------------------------------------------------------------------
                    .386p

;-- KONSTANTE --------------------------------------------------------------
    b               equ  byte ptr
    w               equ  word ptr
    d               equ dword ptr
    o               equ offset
    FILESIZE        equ 1000            ; Dateigröße dieses Files (Paragraphen)

;-- STACK ------------------------------------------------------------------
    STACK_SEG       segment STACK USE16
                    db 1000 dup (?)
    STACK_SEG       ends

;-- DATEN ------------------------------------------------------------------
    DATA            segment USE16
    init_pmode      label word
        pmode_ofs   dw ?
        pmode_seg   dw ?
    dpmi_mem        dw ?

    DPMI_NotFound   db "Kein DPMI, Programmabbruch !",13,10,36
    DPMI_Version    db "DPMI-Host gefunden, Version: "
                hi  db "??"
                    db "."
                lo  db "??",13,10,36
    DPMI_16         db "DPMI-Host unterstützt nur 16-Bit Programme.",13,10,36
    DPMI_32         db "DPMI-Host unterstützt 32-Bit Programme.",13,10,36
    DPMI_Prozessor  db "Prozessor: ",36
    DPMI_80286      db "80286",13,10,36
    DPMI_80386      db "80386",13,10,36
    DPMI_80486      db "80486",13,10,36
    DPMI_80586      db "Pentium oder höher ...",13,10,36
    DPMI_CPU        dw DPMI_80286, DPMI_80386, DPMI_80486, DPMI_80586
    DPMI_nopmode    db "Fehler bei Protected Mode init ??!!",13,10,36

    save_cs         dw ?
    save_ds         dw ?

    reg_edi         dd 0
    reg_esi         dd 0
    reg_ebp         dd 0
    reg_            dd 0
    reg_ebx         dd 0
    reg_edx         dd 0
    reg_ecx         dd 0
    reg_eax         dd 0
    reg_flags       dw 0
    reg_es          dw 0
    reg_ds          dw 0
    reg_fs          dw 0
    reg_gs          dw 0
    reg_ip          dw 0
    reg_cs          dw 0
    reg_sp          dw 0
    reg_ss          dw 0

    pmode_text      db 13,10
                    db "Dieser Text wurde im Protected Mode über einen",13,10
                    db "simulierten Aufruf des DOS-Interrupts 21h, Funktion 09h",13,10
                    db "ausgegeben ... Bitte eine Taste drücken !",13,10,36

    DATA            ends

;-- CODE -------------------------------------------------------------------
    CODE            segment USE16
                    assume CS:CODE, DS:DATA, SS:STACK_SEG

                    include showdez.inc
                    include ehandler.inc
    start:

    ; den für die exe-Datei reservierten Speicherplatz einschränken
    ; Voraussetzung, daß Speicher über DOS-Fkt. 48h reserviert werden kann
                    mov ah,4Ah
                    mov bx,FILESIZE
                    ; es = Segmentadresse des Speicherbereiches,
                    ;      zeigt bei exe-Dateien immer auf den Anfang des Programms
                    int 21h
                    memCheckError

                    mov ax,DATA
                    mov ds,ax
                    mov es,ax
                    mov save_ds,ax
                    mov ax,CODE
                    mov save_cs,ax
    ;
    ; Feststellen, ob überhaupt ein DPMI-Host im System vorhanden ist
    ; falls ja, Real-Mode zu Protected-Mode Einsprungspunkt ermitteln
    ;
                    mov ax,1687h
                    int 2fh

                    cmp ax,0
                    je DPMI_ok
    ;
    ; kein DPMI-Host, Fehlermeldung und zurück zu DOS
    ;
    DPMI_Error:
                    mov ah,09
                    mov dx,o DPMI_NotFound
                    int 21h

                    mov ax,4c01h
                    int 21h

    ;-----------------------------------------------------------------------
    ; DPMI Host gefunden
    ; Init.-Adresse sichern
    ;
    DPMI_ok:
                    mov pmode_seg,es
                    mov pmode_ofs,di
                    mov dpmi_mem,si
    ;
    ; Versionsnummer (DH.DL) ins Datensegment ausgeben
    ;
                    mov ax,dx
                    shr ax,8
                    mov di,o hi
                    showdez

                    mov ax,dx
                    and ax,0FFh
                    mov di,o lo
                    showdez

                    mov ah,09
                    mov dx,o DPMI_Version
                    int 21h
    ;
    ; Ausgabe, ob DPMI-Host 32-Bit Programme unterstützt (BX, Bit 0 = 1)
    ;
                    test bx,1
                    jnz DPMI_allow32

    DPMI_only16:
                    mov ah,09
                    mov dx,o DPMI_16
                    int 21h
                    jmp DPMI_ShowProzessor

    DPMI_allow32:
                    mov ah,09
                    mov dx,o DPMI_32
                    int 21h
    ;
    ; Ausgabe des Prozessortypes
    ;
    DPMI_ShowProzessor:
                    mov ah,09
                    mov dx,o DPMI_Prozessor
                    int 21h

                    sub cl,2
                    cmp cl,3
                    jbe cpu_ok
                    mov cl,3

    cpu_ok:
                    and cx,0FFh
                    shl cx,1
                    add cx,o DPMI_CPU
                    mov di,cx
                    mov dx,[di]

                    mov ah,09
                    int 21h
    ;
    ; Reservieren von Speicher für den DPMI-Host (dpmi_mem Paragraphen)
    ;
                    mov ah,48h
                    mov bx,dpmi_mem
                    int 21h
                    memCheckError
    ;
    ; Initialisierung des Protected Mode
    ;
                    mov es,ax
                    call dword ptr ds:init_pmode
    ;
    ; Protected Mode init erfolgreich ?
    ;
                    jnc pmode
                    mov ah,09
                    mov dx,o DPMI_nopmode
                    int 21h

                    mov ax,4c01h
                    int 21h

    ;-----------------------------------------------------------------------
    ; ab hier im Protected Mode
    pmode:

    ;
    ; über DPMI, Realmode Interrupt simulieren und Text über Fkt. 09 des
    ; Interrupt 21h ausgeben ...
    ;
                    mov reg_eax,0900h
                    mov reg_edx,o pmode_text
                    mov ax,save_ds
                    mov reg_ds,ax
                    pushf
                    pop ax
                    mov reg_flags,ax
                    
                    mov ax,0300h
                    mov bl,21h
                    mov bh,0
                    mov cx,0
                    mov edi,o reg_edi
                    push ds
                    pop es
                    int 31h
                    jc quit
    ;
    ; auf Taste warten
    ;
                    mov ah,0
                    int 16h
    ;
    ; Zurück zu DOS / Deinit Protected Mode
    ;
    quit:
                    mov ax,4c00h
                    int 21h
    CODE            ends
                    end start
    
