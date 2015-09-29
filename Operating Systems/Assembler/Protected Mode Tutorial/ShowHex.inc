    ;------------------------------------------------------------------------
    ; Output X nach ES:DI
    ;------------------------------------------------------------------------
   
    ShowChar    macro X
                push ax
                mov al,X
                stosb
                pop ax
                endm

    ;------------------------------------------------------------------------
    ; hexadezimale Darstellung von AL
    ;
    ; AL = Quellzahl
    ; ES:DI = Ziel (2 Byte Buffer)
    ;------------------------------------------------------------------------
                HexNr db "0123456789ABCDEF"
    ShowHexByte proc
                push ax 
                push bx 
                push cx
                and ax,0FFh						; ausmaskieren, da nur AL Daten enthält			
                mov cx,ax                   	; AX sichern
    
                mov bx,ax
                shr bx,4							; oberes Nibbel betrachten

                mov al,[HexNr+bx]				; oberes Nibbel als Index in HexNr
                stosb								; Inhalt von AL nach ES:DI

                mov bx,cx
                and bx,0Fh							; nur die unteren 4 Bits von AL betrachten
                mov al,[HexNr+bx]				; untere 4 Bits von AL als Index in HexNr
                stosb								; Inhalt von AL nach ES:DI

                pop cx 
                pop bx
                pop ax
                ret
    ShowHexByte endp

    ;------------------------------------------------------------------------
    ; hexadezimale Darstellung von AX
    ;
    ; AX = Quellzahl
    ; ES:DI = Ziel (4 Byte Buffer)
    ;------------------------------------------------------------------------
    ShowHexWord proc
                push ax
                rol ax,8							; die oberen 8 Bits von AX in die unteren 8 Bits rotieren
                call ShowHexByte
                rol ax,8							; die ursprünglichen unteren 8 Bits von AX zurückrotieren
                call ShowHexByte
                pop ax
                ret
    ShowHexWord endp

    ;------------------------------------------------------------------------
    ; hexadezimale Darstellung von EAX
    ;
    ; EAX = Quellzahl
    ; ES:DI = Ziel (8 Byte Buffer)
    ;------------------------------------------------------------------------
    ShowHexDword proc
                push eax
                rol eax,16							; die oberen 16 Bits von EAX in die unteren 16 Bits rotieren
                call ShowHexWord
                rol eax,16
                call ShowHexWord					; die ursprünglichen unteren 16 Bits von EAX zurückrotieren
                pop eax
                ret
    ShowHexDword endp
