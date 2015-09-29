;---------------------------------------------------------------------------
; PM.ASM - Protected Mode Init / Deinit
;---------------------------------------------------------------------------
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:    05.12.96
; Letze Änderungen: 06.12.96
;                   29.03.97
;---------------------------------------------------------------------------
                    .386p

;-- PUBLIC -----------------------------------------------------------------
    public          pm_init

;-- KONSTANTE --------------------------------------------------------------
    BELL            equ 7
    b               equ byte ptr
    w               equ word ptr
    d               equ dword ptr
    o               equ offset

    ; Selektoren
    sNIL            equ 0000000000000000b
    sCODE           equ 0000000000001000b
    sCODE_RETURN    equ 0000000000010000b
    sDATA           equ 0000000000011000b
    sSTACK          equ 0000000000100000b
    sSCREEN         equ 0000000000101000b
    sTASK_FIRST     equ 0000000000110000b
    sTASK_THIS      equ 0000000000111000b

;-- STRUKTUREN -------------------------------------------------------------
    GDTSTRUC        struc
        limit       dw ?                        ; Größe der GDT
        base        dd ?                        ; Basisadresse der GDT
    GDTSTRUC        ends

    TSS_STRUC       struc
        tbacklink   dw ?
                    dw 0
        tesp0       dd ?
        tss0        dw ?
                    dw 0
        tesp1       dd ?
        tss1        dw ?
                    dw 0
        tesp2       dd ?
        tss2        dw ?
                    dw 0
        tcr3        dd ?
        teip        dd ?
        teflags     dd ?
        teax        dd ?
        tecx        dd ?
        tedx        dd ?
        tebx        dd ?
        tesp        dd ?
        tebp        dd ?
        tesi        dd ?
        tedi        dd ?
        tes         dw ?
                    dw 0
        tcs         dw ?
                    dw 0
        tss         dw ?
                    dw 0
        tds         dw ?
                    dw 0
        tfs         dw ?
                    dw 0
        tgs         dw ?
                    dw 0
        tldtr       dw ?
                    dw 0
        tt          dw ?
        tiopbb      dw ?
    TSS_STRUC       ends
                                                                    
;-- MAKROS -----------------------------------------------------------------
    ; legt eine Struktur für einen Deskriptor an
    makeDESKRIPTOR  macro LIMIT, BASE_015, BASE_1623, ACCESS, ATTR, BASE_2431
                    dw LIMIT
                    dw BASE_015
                    db BASE_1623
                    db ACCESS
                    db ATTR
                    db BASE_2431
                    endm

    ; legt eine Struktur für ein Call-Gate an
    makeCALLGATE    macro OFFSET_LOW, OFFSET_HIGH, SELEKTOR, COUNT, ATTR
                    dw OFFSET_LOW
                    dw SELEKTOR
                    db COUNT
                    db ATTR
                    dw OFFSET_HIGH
                    endm

    ; wandelt übergebenes SEGMENT:OFFSET Paar in 24 Bit Adresse um und
    ; trägt die ermittelte Adresse als Basisadresse in den Deskriptor N
    ; der GDT ein
    baseDESKRIPTOR  macro N, SEGMENT, OFFSET
                    mov ax,SEGMENT
                    and eax,0FFFFh
                    shl eax,4
                    add ax,OFFSET

                    mov w [GDT+8*N+2],ax
                    shr eax,16
                    mov b [GDT+8*N+4],al
                    endm

    ; legt eine Struktur für ein Interrupt-Gate an
    makeIDT         macro OFFSET, SELEKTOR, ATTR
                    dw OFFSET
                    dw SELEKTOR
                    db 0
                    db ATTR
                    dw 0
                    endm

;-- DATEN ------------------------------------------------------------------
    daten           segment USE16 public
                    gdt_adr GDTSTRUC ?
                    idt_adr GDTSTRUC ?

    ; TSS für diese Routine
    TSS_THIS_TASK   TSS_STRUC ?                     ; Zur Laufzeit init.

    ; TSS für aufrufende Routine
    TSS_FIRST_TASK  TSS_STRUC ?                     ; Zur Laufzeit init.
    TSS_SIZE        equ $-TSS_FIRST_TASK

    ; Globale Deskriptor Tabelle
    GDT             label byte
                    ; wird grundsätzlich nicht benutzt
                    makeDESKRIPTOR 0,0,0,0,0,0

                    ; Codesegment
                    makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0

                    ; Codesegment für Rücksprung
                    makeDESKRIPTOR 0FFFFh,?,?,10011010b,0,0

                    ; Datensegment
                    makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0

                    ; Stacksegment
                    makeDESKRIPTOR 0FFFFh,?,?,10010010b,0,0

                    ; für VRAM (0b8000h)
                    makeDESKRIPTOR 4000,8000h,0Bh,10010010b,0,0

                    ; TSS-Deskriptor für ersten Task
                    makeDESKRIPTOR TSS_SIZE,?,?,10001001b,0,0

                    ; TSS-Deskriptor für diesen Task
                    makeDESKRIPTOR TSS_SIZE,?,?,10001001b,0,0       
    GDTSIZE         equ $-GDT

    ; Interrupt Deskriptor Tabelle
    IDT             label byte
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
                    makeIDT IRQ_00, sCODE, 10001110b
                    makeIDT IRQ_16, sCODE, 10001110b
                    makeIDT IRQ_17, sCODE, 10001110b
    IDTSIZE         equ $-IDT

    ; für Rückkehr in Realmode
    saveSS          dw ?
    saveDS          dw ?

    ; für Ausgabe der allgemeinen Register
    r_eax           dd ?
    r_ebx           dd ?
    r_ecx           dd ?
    r_edx           dd ?
    r_esi           dd ?
    r_edi           dd ?
    r_esp           dd ?
    r_ebp           dd ?
    r_eip           dd ?
    r_cs            dw ?
    r_ds            dw ?
    r_es            dw ?
    r_fs            dw ?
    r_gs            dw ?
    r_ss            dw ?

    exception       db ?

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
                    db "(Segment-Überlauf des Coprozessors) "
                    db "(TSS ungültig)                      "
                    db "(Segment nicht im Hauptspeicher)    "
                    db "(Stack-Überlauf)                    "
                    db "(allgemeine Schutzverletzung)       "
                    db "(Paging Ausnahmezustand)            "
                    db "(reserviert)                        "
                    db "(Coprozessor-Fehler)                "
                    db "(Ausrichtungfehler)                 "
    
    ; Exception Text, wird nach der Rückschaltung in den rmode über
    ; Funktion 09h des Int 21h ausgegeben
    txt             db BELL, "Exception "
    NR              db "00 "
    oNAME           db eintrag_laenge dup (' ')
                    db 13,10,10
                    db "cs = "
    oCS             db "0000     "
                    db "eip= "
    oEIP            db "00000000 ", 13, 10
                    db "ss = "
    oSS             db "0000     "
                    db "esp= "
    oESP            db "00000000 ", 13, 10
                    db "eax= "
    oEAX            db "00000000 "
                    db "ebx= "
    oEBX            db "00000000 "
                    db "ecx= "
    oECX            db "00000000 "
                    db "edx= "
    oEDX            db "00000000 ", 13, 10
                    db "esi= "
    oESI            db "00000000 "
                    db "edi= "
    oEDI            db "00000000 "
                    db "ebp= "
    oEBP            db "00000000 ", 13, 10
                    db "ds = "
    oDS             db "0000     "
                    db "es = "
    oES             db "0000     "
                    db "fs = "
    oFS             db "0000     "
                    db "gs = "
    oGS             db "0000     ", 13,10
                    db "$"
    nul             dd ?
                    ends

;-- PROGRAMMCODE -----------------------------------------------------------
                    assume cs:pmcode, ds:daten
    pmcode          segment use16

    align 4
    ;- pm_init ----------------------------------------------------------------------
    ; init pmode, führt alle notwendigen Vorbereitungen durch und kehrt
    ; zurück zum Aufrufer.
    ;
    ; In:   nil
    ; Out:  ds,fs,gs = Datensegment Selektor (sDATA)
    ;       es       = Selektor für SCREEN (sSCREEN)
    ;       ss       = Stacksegment Selektor (sSTACK)
    ;       ax       = Selektor für diesen Task (Rückkehr Möglichkeit)
    ; Chg:  all
    ;-----------------------------------------------------------------------
    pm_init         proc far
                    cld

                    mov ax,daten
                    mov ds,ax
                    mov es,ax
                    mov saveDS,ax

    ; Stacksegment sichern
                    mov ax,ss
                    mov saveSS,ax

    ; Rücksprungadresse vom Stack holen und für Rücksprung sichern
                    pop bx                      ; Offset
                    pop cx                      ; Segmentadresse
                    baseDESKRIPTOR 2, cx, 0

    ; Basisadressen Init
                    baseDESKRIPTOR 1, cs, 0
                    baseDESKRIPTOR 3, ds, 0
                    baseDESKRIPTOR 4, ss, 0

                    mov ax,ds
                    and eax,0FFFFh
                    shl eax,4
                    add ax,offset TSS_FIRST_TASK
                    mov w [GDT+8*6+2],ax
                    shr eax,16
                    mov b [GDT+8*6+4],al

                    mov ax,ds
                    and eax,0FFFFh
                    shl eax,4
                    add ax,offset TSS_THIS_TASK
                    mov w [GDT+8*7+2],ax
                    shr eax,16
                    mov b [GDT+8*7+4],al

    ; TSS_FIRST_TASK init
                    mov di,offset TSS_FIRST_TASK

                    mov eax,esp
                    and eax,0FFFFh

                    mov [di+tbacklink],0
                    mov [di+tesp0],eax
                    mov [di+tss0],sSTACK
                    mov [di+tesp1],eax
                    mov [di+tss1],sSTACK
                    mov [di+tesp2],eax
                    mov [di+tss2],sSTACK
                    mov [di+tcr3],0

                    and ebx,0FFFFh
                    mov [di+teip],ebx

                    pushfd
                    pop ebx
                    mov [di+teflags],ebx
                    mov [di+teax],sTASK_THIS
                    mov [di+tecx],0
                    mov [di+tedx],0
                    mov [di+tebx],0

                    mov [di+tesp],eax
                    mov [di+tebp],0
                    mov [di+tesi],0
                    mov [di+tedi],0
                    mov [di+tes],sDATA
                    mov [di+tcs],sCODE_RETURN
                    mov [di+tss],sSTACK
                    mov [di+tds],sDATA
                    mov [di+tfs],sDATA
                    mov [di+tldtr],0
                    mov [di+tt],0
                    mov [di+tiopbb],0

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
    ; wurden z.B. keine Handler für Hardware-Interrupts bereitgestellt)
                    cli
                    in al,70h
                    or al,80h
                    out 70h,al

    ; Adresse der IDT setzen
                    mov ax,IDTSIZE
                    mov [idt_adr.limit],ax

                    mov ax,seg IDT
                    and eax,0FFFFh
                    shl eax,4
                    add ax,offset IDT
                    mov [idt_adr.base],eax
                    LIDT idt_adr

    ; für rmode Rücksprung, Codesegment-Adresse unter <rmode_cs> sichern
                    mov ax,cs
                    mov cs:rmode_cs,ax
                    mov cs:rmode_cs2,ax

    ; Protected Mode aktivieren
                    mov eax,cr0
                    or eax,1
                    mov cr0,eax

    ; "FAR-JMP" zu Selektor:offset (entspricht jmp sCODE:pmode)
                    db 0eah
                    dw offset cs:pmode
                    dw sCODE

    pmode:
    ; Selektor für Stack Segment setzten
                    mov ax,sSTACK
                    mov ss,ax

    ; Selektor Register vorbereiten
                    mov ax,sDATA
                    mov ds,ax
                    mov fs,ax
                    mov gs,ax
                    mov es,ax

                    mov ax,sTASK_THIS
                    LTR ax

    ; "FAR"-JMP zum Aufrufer (Task-Wechsel !!!)
                    db 0eah
                    dw 0
                    dw sTASK_FIRST

    ; Durch obige Befehlsequenz wird ein Task-Wechsel zum aufrufenden
    ; Programms durchgeführt. Die Werte für die Register werden dabei dem
    ; TSS-Segment entnommen (wurde ab Zeile 323 mit Startwerten initialisiert)

    ; nachdem das ausgeführte Programm einen Taskwechsel via JMP auslöst,
    ; "landet" der Prozessor wieder hier (alle Register besitzen wieder ihren
    ; alten Zustand) und setzt seine Arbei fort
    ; In diesem Fall: Rückkehr zum Realmode

    ;- pm_deinit -----------------------------------------------------------
    ; deinit pmode, schaltet zurück in den pmode
    ;
    ; In:   nil
    ; Out:  nil
    ; Chg:  all
    ;-----------------------------------------------------------------------
                    mov ax,sDATA
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax
                    mov ss,ax

    ; Interrupt-Deskriptortabelle wieder auf Realmode-Standard
                    mov [idt_adr.base],0
                    mov [idt_adr.limit],03ffh
                    LIDT idt_adr

    ; Protected Mode Aus ...
                    mov eax,cr0
                    and eax,NOT 1
                    mov cr0,eax

                    db 0eah
                    dw offset rmode
    rmode_cs        dw ?

    rmode:
    ; Stack Segmentregister auf alten Wert
                    mov ax,saveSS
                    mov ss,ax

    ; Interrupts an
                    in al,70h
                    or al,not 80h
                    out 70h,al
                    sti

    ; zurück zu DOS
                    mov ax,4c00h
                    int 21h
                    endp

                    include tool.inc

;-- EXCEPTION-HANDLER (int 0..17) ------------------------------------------
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
                    jmp quit_all
    
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
                    jmp quit_all

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
                    mov ax,sDATA
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax
                    mov ss,ax

    ; Paging Aus ...
                    mov eax,cr0
                    and eax,01111111111111111111111111111111b
                    mov cr0,eax

                    mov eax,0
                    mov cr3,eax
    
    ; Pmode Aus ...
                    mov eax,cr0
                    and eax,NOT 1
                    mov cr0,eax

                    db 0eah
                    dw offset rmode2
    rmode_cs2       dw ?

    rmode2:
    ; Interrupt-Deskriptortabelle wieder auf Realmode-Standard
                    mov [idt_adr.base],0
                    mov [idt_adr.limit],03ffh
                    LIDT idt_adr

    ; Stack Segmentregister auf alten Wert
                    mov ax,saveSS
                    mov ss,ax
                    mov sp,01000h

    ; Interrupts an ...
                    in al,70h
                    and al,not 80h
                    out 70h,al
                    sti

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
                    ends

                    end


