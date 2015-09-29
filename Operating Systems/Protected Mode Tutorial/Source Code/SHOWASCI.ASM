;----------------------------------------------------------------------------
; SHOWASCI.ASM
;
; Ausgabe eines aus mehreren Segmenten zusammengesetzten Datenblocks,
; als ASCII-Zeichen (nicht ausgebare Steuerzeichen werden duch '_' ersetzt)
;
; (c) Copyright 1996, 1997 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:     17.09.96
; Letze Änderungen:  18.09.96
;                    23.03.97
;----------------------------------------------------------------------------

                    public showasci

    CODE            segment PARA PUBLIC 'CODE'
                    assume cs:CODE

    ;
    ; Ausgabe des Datenblocks mit den folgenden Eigenschaften als ASCII Zeichen
    ;
    ; ds:si - auszugebender Speicherbereich, erstes Segment
    ; dx:ax - Anzahl auszugebender Bytes
    ;         (dx= Anzahl 64 KB Bloecke, ax= Rest)  
    showasci        proc

    ; Anzahl Bytes im kleinem Block in di sichern
                    mov di,ax

    ; auszugebender Block > 64 KB ?
                    or dx,dx
                    jz kleiner_block

    ; Ausgabe von <dx> 64 KB Datenblöcke
    outerloop:
                    mov cx,0

    innerloop:
                    lodsb
                    cmp al,32
                    jae showchar                    ; "sichtbares" Zeichen ?
                    mov al,'þ'

    showchar:
                    push dx

    ; ausgabe al
                    mov dl,al
                    mov ah,02
                    int 21h

                    pop dx

                    dec cx
                    jnz innerloop

    ; Segmentregister auf nächsten Block aktualisieren
                    mov cx,es
                    add cx,01000h                   ; (4096d)
                    mov es,cx

                    dec dx
                    jnz outerloop

    ; einen kleinen Block (< 64 KB) ausgeben
    kleiner_block:
                    mov cx,di

    ausgabe:
                    lodsb
                    cmp al,32
                    jae showchar2
                    mov al,' '

    showchar2:

    ; ausgabe al
                    mov dl,al
                    mov ah,02
                    int 21h

                    dec cx
                    jnz ausgabe

    quit:
                    ret
                    endp
    CODE            ends
                    end

