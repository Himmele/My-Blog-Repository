;----------------------------------------------------------------------------
; RM_01.ASM
;
; Demonstration des Zugriffs auf große Speicherbereiche (> 64KB),
; innerhalb des DOS-Speicherbereichs (0-640kb) mittels FAR-Zeiger.
;
; - reservieren eines 100000 Byte großen Speicherbereichs auf dem DOS-Heap
; - Ausgabe des orginalen Speicherinhalts des reservierten Bereichs
;   mit externer <SHOWASCI> Routine
; - Füllen des reservierten Bereiches mit konstantem Wert
; - Ausgabe des reservierten Bereiches
;
; (c) Copyright 1996, 1997 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:     17.09.96
; Letze Aenderungen: 22.10.96
;                    23.03.97
;----------------------------------------------------------------------------
                    include showasci.ext

    FILESIZE_PARA   equ 1000                        ; Größe der exe-datei in
                                                    ; paragraphen

    ; Standard Stack Segment
    STACKS          segment PARA STACK 'STACK'
                    mystack db 1000h dup (?)
    STACKS          ends

    CODE            segment para public 'CODE'
                    assume cs:CODE, ss:STACKS

    ; die Adresse des Datenblocks, die DOS zurückliefert
    datenblock_adr  dw ?
    msgFILL         db 13,10
                    db "Fuellen von 100000 Byte mit dem ASCII-Zeichen '@' ...",13,10,36
    msgPRESSKEY     db "Fertig !",13,10,"Taste druecken, um gefuellten Bereich zu sehen ...",13,10,36
    msgINTRO        db 13,10
                    db "Anzeigen von 100000 Bytes als ASCII-Code",13,10
                    db "Taste druecken, um Bereich zu sehen ...",36

    ; Errorhandler (makro: memCheckError) einbinden
                    include ehandler.inc

    start:
    ; den für die exe-Datei reservierten Speicherplatz einschränken
    ; Voraussetzung, daß überhaupt Speicher über Fkt. 48h reserviert werden kann
                    mov ah,4Ah
                    mov bx,FILESIZE_PARA
                    ; es = Segmentadresse des Speicherbereiches,
                    ;      zeigt bei exe-Dateien immer auf den Anfang
                    ;      des Programms
                    int 21h
                    memCheckError

    ; Reservieren des Speicherbereichs über DOS-Fkt. 48h
    ; Abbruch bei Fehler
                    mov ah,48h
                    mov bx,6250                     ; = 100000 / 16
                    int 21h
                    memCheckError
                    mov cs:datenblock_adr,ax

    ; Nachricht ausgeben und auf Taste warten
                    push cs
                    pop ds
                    mov dx,offset msgINTRO
                    mov ah,09
                    int 21h

                    mov ah,0
                    int 16h

    ; Ausgabe des Speicherbereichs mit Fkt.: showasci
                    mov ds,ax
                    mov di,0
                    mov dx,1
                    mov ax,100000-65536
                    call showasci

    ; "Füllt gerade" Nachricht ausgeben
                    push cs
                    pop ds
                    mov dx,offset msgFILL
                    mov ah,09
                    int 21h
                
    ; Füllen des Speichers, wie im Textbeispiel
                    mov ax,cs:datenblock_adr
                    mov es,ax
                    mov bx,0
	
    ; mit dem Inhalt von al wird der Speicherbereich gefüllt
                    mov al,'@'
    
    ; füllen des Speicherbereichs (100000 Bytes) mit al
    ; ersten Block bearbeiten
                    mov cx,0

    inLoop1:
                    mov es:[bx],al      ; Byte schreiben
                    inc bx              ; Offsetadresse erhöhen
                    dec cx
                    jnz inLoop1
    
    ; Segmentregister auf neuen Block aktualisieren
                    mov dx,es
                    add dx,01000h       ; (4096d)
                    mov es,dx
    
    ; zweiten Block bearbeiten
                    mov cx,100000-65536

    inLoop2:
                    mov es:[bx],al
                    inc bx
                    dec cx
                    jnz inLoop2

                    push cs
                    pop ds
                    mov dx,offset msgPRESSKEY
                    mov ah,09
                    int 21h

    ; auf Taste warten
                    mov ah,0
                    int 16h

    ; Ausgabe des Speicherbereichs mit Fkt.: showasci
                    mov ax,cs:datenblock_adr
                    mov ds,ax
                    mov si,0

                    mov dx,1
                    mov ax,100000-65536
                    call showasci

    ; freigeben des Speichers
                    mov bx,cs:datenblock_adr
                    mov es,bx

                    mov ah,49h
                    int 21h
                    memCheckError

    ; zurück zu DOS
                    mov ax,4c00h
                    int 21h
    CODE            ends
                    end start

