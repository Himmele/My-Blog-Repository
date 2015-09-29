;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; TOOL.INC (V2.0, PMODE)
;
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:    23.03.96
; Letze Žnderungen: 19.01.97
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

    ;Ä ShowChar ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    ; Output z nach es:di
    ;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    ShowChar    macro z
                push ax
                mov al,z
                stosb
                inc di
                pop ax
                endm

    ;Ä ShowHexByte ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    ; hexadezimale Darstellung von AL
    ;
    ; In:      al = Quellzahl
    ;       es:di = Ziel (2 Byte Buffer)
    ; Out:  nil
    ; Chg:  nil
    ;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
                HexNr db "0123456789ABCDEF"
    ShowHexByte proc
                push ax bx cx
                and ax,0ffh
                mov cx,ax                   ; Save ax
    
                mov bx,ax
                shr bx,4

                mov al,[HexNr+bx]
                stosb
                inc di

                mov bx,cx
                and bx,0fh
                mov al,[HexNr+bx]
                stosb
                inc di

                pop cx bx ax
                ret
                endp

    ;Ä ShowHexWord ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    ; hexadezimale Darstellung von AX
    ;
    ; In:      ax = Quellzahl
    ;       es:di = Ziel (4 Byte Buffer)
    ; Out:  nil
    ; Chg:  nil
    ;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    ShowHexWord proc
                push ax
                rol ax,8
                call ShowHexByte
                rol ax,8
                call ShowHexByte
                pop ax
                ret
                endp

    ;Ä ShowHexDword ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    ; hexadezimale Darstellung von EAX
    ;
    ; In:     eax = Quellzahl
    ;       es:di = Ziel (8 Byte Buffer)
    ; Out:  nil
    ; Chg:  nil
    ;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
    ShowHexDword proc
                push eax
                rol eax,16
                call ShowHexWord
                rol eax,16
                call ShowHexWord
                pop eax
                ret
                endp

