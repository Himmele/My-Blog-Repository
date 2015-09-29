;---------------------------------------------------------------------------
; PM_05.ASM - Protected Mode Init / Deinit mit VCPI
;---------------------------------------------------------------------------
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:    18.12.96
; Letze Änderungen: 03.01.97
;---------------------------------------------------------------------------
                    .386p

;-- KONSTANTE --------------------------------------------------------------
    b               equ byte ptr
    w               equ word ptr
    d               equ dword ptr
    o               equ offset
    FILESIZE        equ 1000            ; Dateigröße dieses Files (Paragraphen)

;-- STACK ------------------------------------------------------------------
    STACK_SEG       segment STACK USE16
                    db 1000 dup (?)
    STACK_SEG       ends

;-- STRUKTUREN -------------------------------------------------------------
    GDTSTRUC        struc
        limit       dw ?                        ; Größe der GDT
        base        dd ?                        ; Basisadresse der GDT
    GDTSTRUC        ends

    ; legt eine Struktur für einen Deskriptor an
    makeDESKRIPTOR  macro LIMIT, BASE_015, BASE_1623, ACCESS, ATTR, BASE_2431
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
                add eax,OFFSET

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

    ; legt eine 4 Byte lange Struktur an, die als Page-Table Eintrag
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

    TSS_STRUC       struc
        tbacklink   dw 0
                    dw 0
        tesp0       dd 0
        tss0        dw 0
                    dw 0
        tesp1       dd 0
        tss1        dw 0
                    dw 0
        tesp2       dd 0
        tss2        dw 0
                    dw 0
        tcr3        dd 0
        teip        dd 0
        teflags     dd 0
        teax        dd 0
        tecx        dd 0
        tedx        dd 0
        tebx        dd 0
        tesp        dd 0
        tebp        dd 0
        tesi        dd 0
        tedi        dd 0
        tes         dw 0
                    dw 0
        tcs         dw 0
                    dw 0
        tss         dw 0
                    dw 0
        tds         dw 0
                    dw 0
        tfs         dw 0
                    dw 0
        tgs         dw 0
                    dw 0
        tldtr       dw 0
                    dw 0
        tt          dw 0
        tiopbb      dw 0
    TSS_STRUC       ends

;-- DATEN ------------------------------------------------------------------
    DATA            segment PUBLIC USE16

    ; Fehlermeldungen
    EMM_NOEMS       db "EMS-Fehler: Kein EMS-Treiber installiert, oder Option NOEMS verwendet.",13,10,36
    EMM_NOALLOC     db "EMS-Fehler: Fehler bei Seitenreservierung.",13,10,36
    EMM_NOFREE      db "EMS-Fehler: Fehler bei Seitenfreigabe.",13,10,36
    VCPI_NOTFOUND   db "VCPI-Fehler: kein VCPI vorhanden.",13,10,36
    VCPI_FKT01FAILED db"VCPI-Fehler: unmoeglich pmode-interface zu initialisieren.",13,10,36

    ; Buffer für Ausgabe max. Anzahl 4K Pages
    zahl            db ?,?,?,?,?,?,?,?
                    db "h 4KB Speicherseiten verfügbar.",13,10,36

    ; Handle für ems-Seite
    handle          dw ?

    ; Task-State-Segment für VCPI Protected Mode Init
    TSS_MYTASK      TSS_STRUC ?
    TSS_SIZE        equ $-TSS_MYTASK

    ; GDTR und IDTR Registerinhalte
    gdt_adr         GDTSTRUC ?
    idt_adr         GDTSTRUC ?

    ; GLOBALE DESKRIPTOR TABELLE
    GDT             label byte
                    makeDESKRIPTOR 0,0,0,0,0,0                  ; wird grundsätzlich nicht benutzt

                    vcpi_init       label byte
                    makeDESKRIPTOR ?,?,?,?,?,?                  ; VCPI-Codesegment
                    makeDESKRIPTOR ?,?,?,?,?,?                  ; für VCPI-Server
                    makeDESKRIPTOR ?,?,?,?,?,?                  ; für VCPI-Server

                    makeDESKRIPTOR 0FFFFh,?,?,  10011010b,0,0   ; Codesegment
                    makeDESKRIPTOR 0FFFFh,?,?,  10010010b,0,0   ; Datensegment
                    makeDESKRIPTOR TSS_SIZE,?,?,10001001b,0,0
                    makeDESKRIPTOR 0FFFFh, 0,0, 10010010b, 01011111b, 0
    GDTSIZE         equ $-GDT

    ; Selektoren für obengenannte Deskriptoren
    sNIL            equ 0000000000000000b
    sVCPI_CODE      equ 0000000000001000b
    sVCPI_USE1      equ 0000000000010000b
    sVCPI_USE2      equ 0000000000011000b

    sCODE           equ 0000000000100000b
    sDATA           equ 0000000000101000b
    sTASK_MY        equ 0000000000110000b
    sDATAZERO       equ 0000000000111000b

    ; INTERRUPT DESKRIPTOR TABELLE
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

    ; Registerbuffer für pmode Init per VCPI
    pm_CR3          dd ?
    pm_GDTR         dd ?
    pm_IDTR         dd ?
    pm_LDTR         dw sNIL
    pm_TR           dw sTASK_MY
    pm_EIP          dw pmode
                    dw 0
    pm_CS           dw sCODE

    ; Registerbuffer für rmode Init per VCPI
    rm_init         dq 0
    rm_eip          dd 0
    rm_cs           dd 0
    rm_reserviert   dd 0
    rm_esp          dd 0
    rm_ss           dd 0
    rm_es           dd 0
    rm_ds           dd 0
    rm_fs           dd 0
    rm_gs           dd 0
                       
    DATA            ends

;-- CODE --------------------------------------------------------------------
    CODE            segment PUBLIC USE16
                    assume CS:CODE, DS:DATA, SS:STACK_SEG

    ; Far-Call aus Protected Mode zum VCPI-Server
    vcpi_callback   dd ?
                    dw sVCPI_CODE

    ; Adresse der rm_callback Struktur (rm_eip)
    rm_initadr      dd ?

    ; PROBLEM: Page-Tables müssen immer an einer durch 4096 teilbaren Adresse
    ;          beginnen!
    ;          Es ist (nach meinem aktuellen Wissenstand, 29.11.1996)
    ;          nicht möglich eine solche Ausrichtung von Daten innerhalb
    ;          eines Assemblerprogr. zu erreichen (z.B. durch ALIGN 12)
    ;          Dieses Programm reserviert darum 8192 Byte je Page-Table und
    ;          sucht zur Laufzeit eine durch 4096 teilbare Adresse (in diesem
    ;          Speicherbereich) werden die page-tables angelegt ...
    pagedir_adr_DOS dw ?                    ; Segmentadresse des DOS-Speicherbereichs
        pagedir_adr dw ?                    ; durch 4096 teilbare Adresse innerhalb des
                                            ; DOS-Speicherbereichs ...
    pagetab_adr_DOS dw ?
        pagetab_adr dw ?

    ; Errorhandler für DOS-Heap
                    include ehandler.inc

    ; einfache hexadezimale-Ausgabeprozeduren
                    include tool.inc

    ; Routinen für EMS Erkennung einbinden
                    include ems.inc

    ; Routinen für einfache Text-Ausgabe auf stdout
                    include io.inc

    start:
                    cld

    ; den für die exe-Datei reservierten Speicherplatz einschränken
    ; Voraussetzung, daá Speicher über DOS-Fkt. 48h reserviert werden kann
                    mov ah,4Ah
                    mov bx,FILESIZE
                    ; es = Segmentadresse des Speicherbereiches,
                    ;      zeigt bei exe-Dateien immer auf den Anfang des Programms
                    int 21h
                    memCheckError

    ; Reservieren eines 8192 Byte groáen Speicherbereichs über DOS-Fkt. 48h
                    mov ah,48h
                    mov bx,8192 / 16
                    int 21h
                    memCheckError
                    mov cs:pagedir_adr_DOS,ax

    ; Reservieren eines 8192 Byte groáen Speicherbereichs über DOS-Fkt. 48h
                    mov ah,48h
                    mov bx,8192 / 16
                    int 21h
                    memCheckError
                    mov cs:pagetab_adr_DOS,ax

    ; Ermittlung einer durch 4096 teilbaren Adresse innerhalb des DOS-
    ; Speicherblockes für PAGE-DIRECTORY
                    movzx eax,cs:pagedir_adr_DOS
                    shl eax,4                       ; eax = phys. Adresse
                    and eax,0FFFFF000h
                    add eax,01000h                  ; nächstes Vielfache von 4096
                    shr eax,4                       ; in Segmentadresse umwandeln
                    mov cs:pagedir_adr,ax

    ; Ermittlung einer durch 4096 teilbaren Adresse innerhalb des DOS-
    ; Speicherblockes für PAGE-TABLE
                    movzx eax,cs:pagetab_adr_DOS
                    shl eax,4
                    and eax,0FFFFF000h
                    add eax,01000h
                    shr eax,4
                    mov cs:pagetab_adr,ax

    ; Adresse der Page-Table im Page-Directory eintragen
                    mov di,cs:pagedir_adr
                    mov es,di
                    mov edi,0

                    movzx eax,w cs:pagetab_adr
                    shl eax,4               ; Pagenummer in Bits (31..12)
                    add eax,1               ; Present Bit= 1
                    stosd

                    mov ax,DATA
                    mov ds,ax
                    mov es,ax

    ; Deskriptoren Basisadressen init.
                    baseDESKRIPTOR 4, cs, 0
                    baseDESKRIPTOR 5, ds, 0

                    mov ax,DATA
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TSS_MYTASK
                    mov w [GDT+8*6+2],ax
                    shr eax,16
                    mov b [GDT+8*6+4],al

;-- EMM --------------------------------------------------------------------
    ; Feststellen, ob EMM vorhanden ...
                    call ems_check
                    cmp ax,1
                    je ems_ok

                    mov dx,o EMM_NOEMS
                    jmp error_and_quit

    ems_ok:
                    writeln "EMS-Manager gefunden."

    ; eine EMS-Seite reservieren,
    ; um event. "schlafenden" VCPI-Server zu "wecken" ...
                    mov dx,seg handle
                    mov ax,o handle
                    mov cx,1
                    call ems_alloc
                    cmp ax,1
                    je ems_allocok

                    mov dx,o EMM_NOALLOC
                    jmp error_and_quit

    ems_allocok:
                    writeln "EMS-Seite reserviert."

;-- VCPI -------------------------------------------------------------------
    ; Test ob VCPI-Service zur Verfügung steht
                    mov ax,0DE00h
                    int 67h
                    cmp ah,0
                    je vcpi_initok

                    mov dx,o VCPI_NOTFOUND
                    jmp error_and_quit

    vcpi_initok:
                    write "VCPI gefunden, Versionsnummer: "
                    add bx,3030h
                    mov dl,bh
                    showcharAL
                    mov dl,'.'
                    showcharAL
                    mov dl,bl
                    showcharAL
                    writeln " "

    ; VCPI-Funktion um Page-Table zu füllen und pmode-Einsprungspunkt
    ; zurückzuliefern
                    mov ax,0DE01h
                    mov di,cs:pagetab_adr
                    mov es,di
                    mov di,0

                    mov si,seg vcpi_init
                    mov ds,si
                    mov si,o vcpi_init

                    int 67h
                    cmp ah,0
                    je getpmodeinterface_ok

                    mov dx,o VCPI_FKT01FAILED
                    jmp error_and_quit

    getpmodeinterface_ok:
                    mov cs:vcpi_callback,ebx
                    writeln "Page-Table gefüllt."

                    mov ax,DATA
                    mov ds,ax
                    mov es,ax

                    mov ax,0DE03h
                    int 67h

                    mov eax,edx

                    mov di,o zahl
                    call ShowHexDword

                    mov dx,o zahl
                    mov ah,09
                    int 21h

        ; alle Interrupts aus (nicht alle Interrupts können behandelt werden,
        ; es wurden keine Handler für Hardware-Interrupts bereitgestellt)
                    cli

    ; Realmode Rückschaltung vorbereiten
                    mov ax,CODE
                    mov w rm_cs,ax

                    mov ax,o rmode
                    and eax,0FFFFh
                    mov rm_eip,eax

                    mov ax,DATA
                    mov w rm_ds,ax
                    mov w rm_es,ax
                    mov w rm_fs,ax
                    mov w rm_gs,ax

                    mov ax,STACK_SEG
                    mov w rm_ss,ax

                    mov eax,esp
                    and eax,0FFFFh
                    mov rm_esp,eax

                    mov ax,seg rm_eip
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o rm_eip
                    mov cs:rm_initadr,eax

    ; mit VCPI in den Protected Mode ...
        ; GDT
                    mov ax,seg GDT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o GDT
                    mov [gdt_adr.base],eax
                    mov ax,GDTSIZE
                    mov [gdt_adr.limit],ax

                    mov ax,seg gdt_adr
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o gdt_adr
                    mov pm_GDTR,eax

        ; IDT
                    mov ax,seg IDT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o IDT
                    mov [idt_adr.base],eax
                    mov ax,IDTSIZE
                    mov [idt_adr.limit],ax

                    mov ax,seg idt_adr
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o idt_adr
                    mov pm_IDTR,eax

        ; CR3
                    movzx eax,w cs:pagedir_adr
                    shl eax,4
                    mov pm_CR3,eax

        ; switch to pmode ...
                    mov ax,0DE0Ch
                    mov si,seg pm_CR3
                    and esi,0FFFFh
                    shl esi,4
                    add esi,o pm_CR3
                    int 67h

;-- AB HIER IM PROTECTED MODE ----------------------------------------------
    pmode:
                    in al,70h
                    or al,80h
                    out 70h,al

                    mov ax,sDATA
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax
                    mov ss,ax

    ; Roter Hintergrund
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,40
                    out dx,al
                    mov al,0
                    out dx,al
                    out dx,al

    waitkey:
                    in al,[60h]
                    cmp al,1
                    jne waitkey

    releasekey:
                    in al,[60h]
                    cmp al,81h
                    jne releasekey

    ; Hintergrund wieder schwarz
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    out dx,al
                    out dx,al

    ; alle Interrupts aus (nicht alle Interrupts können behandelt werden,
    ; es wurden keine Handler für Hardware-Interrupts bereitgestellt)
                    cli
    ;
    ; über VCPI zurück in den Real Mode
    ;
    ; ACHTUNG: 1) sDATAZERO muß Selektor auf 1 MB großes Datensegment
    ;             mit der Basisadresse 0 sein. Weiterhin muß das D-Bit
    ;             im Segmentdeskriptor gesetzt werden, um einen 80386
    ;             kompatiblen Deskriptor anzuzeigen !!!
    ;          2) ss:esp muß die lineare Adresse der rm_init Struktur
    ;             enthalten, wobei esp auf rm_eip (siehe oben) zeigen
    ;             muá und nicht auf rm_init (siehe oben) !!!
    ;          3) die Adresse des VCPI-Servers muß in CS festgehalten werden
    ;             da DS mit sDATAZERO verändert wird.
    ;
                    mov ax,sDATAZERO
                    mov ds,ax
                    mov ss,ax
                    mov esp,cs:rm_initadr
                    mov ax,0DE0Ch
                    call fword ptr cs:vcpi_callback

;-- AB HIER IM REALMODE ----------------------------------------------------
    rmode:
    ; Roter Hintergrund
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    out dx,al
                    out dx,al

    ; EMS-Seite freigeben
                    mov ax,seg handle
                    mov ds,ax
                    mov ax,handle
                    call ems_free
                    cmp ax,1
                    je ems_freeok

                    mov dx,o EMM_NOFREE
                    jmp error_and_quit

    ems_freeok:
                    writeln " "
                    writeln "EMS-Seite wieder freigegeben."

    ; DOS-Speicher freigeben
                    mov bx,cs:pagedir_adr_DOS
                    mov es,bx
                    mov ah,49h
                    int 21h
                    memCheckError

                    mov bx,cs:pagetab_adr_DOS
                    mov es,bx
                    mov ah,49h
                    int 21h
                    memCheckError

    ; Erfolgsmeldung
                    writeln "Programm erfolgreich beendet."

                    mov ax,4c00h
                    int 21h

    error_and_quit:
                    mov ax,DATA
                    mov ds,ax

                    mov ah,09
                    int 21h

    ; DOS-Speicher freigeben
                    mov bx,cs:pagedir_adr_DOS
                    mov es,bx
                    mov ah,49h
                    int 21h
                    memCheckError

                    mov bx,cs:pagetab_adr_DOS
                    mov es,bx
                    mov ah,49h
                    int 21h
                    memCheckError

                    mov ax,4c01h
                    int 21h

;-- INTERRUPT - EXCEPTIONS (00-17) -----------------------------------------
; WERDEN DURCH UNTERSCHIEDLICHE FARBEN GEKENNZEICHNET
; DAZU WIRD DER EINTRAG IM VGA-DAC FÜR DIE ERSTE FARBE (HINTERGRUND, RAHMEN
; DES BILDSCHIRMS) VERÄNDERT.
    IRQ_00:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,63
                    out dx,al
                    out dx,al
                    out dx,al
                    hlt

    IRQ_01:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,33
                    out dx,al
                    out dx,al
                    out dx,al
                    hlt

    IRQ_02:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,13
                    out dx,al
                    out dx,al
                    out dx,al
                    hlt

    IRQ_03:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,63
                    out dx,al
                    mov al,0
                    out dx,al
                    out dx,al
                    hlt

    IRQ_04:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,33
                    out dx,al
                    mov al,0
                    out dx,al
                    out dx,al
                    hlt

    IRQ_05:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,13
                    out dx,al
                    mov al,0
                    out dx,al
                    out dx,al
                    hlt

    IRQ_06:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    mov al,63
                    out dx,al
                    mov al,0
                    out dx,al
                    hlt

    IRQ_07:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    mov al,33
                    out dx,al
                    mov al,0
                    out dx,al
                    hlt

    IRQ_08:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    out dx,al
                    mov al,10
                    out dx,al
                    hlt

    IRQ_09:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    out dx,al
                    mov al,33
                    out dx,al
                    hlt

    IRQ_10:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    out dx,al
                    mov al,13
                    out dx,al
                    hlt

    IRQ_11:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,63
                    out dx,al
                    out dx,al
                    mov al,0
                    out dx,al
                    hlt

    IRQ_12:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,33
                    out dx,al
                    out dx,al
                    mov al,0
                    out dx,al
                    hlt

    IRQ_13:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    out dx,al
                    mov al,63
                    out dx,al
                    hlt

    IRQ_14:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    mov al,63
                    out dx,al
                    out dx,al
                    hlt

    IRQ_16:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    mov al,63
                    out dx,al
                    out dx,al
                    hlt

    IRQ_17:
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    out dx,al
                    mov al,33
                    out dx,al
                    out dx,al
                    hlt

    CODE            ends
                    end start



