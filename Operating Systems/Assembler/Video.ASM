.MODEL TINY
.CODE
			
START:			mov ah,15d		; aktuelle Bildschirmseite nach BH
			int 10h			
			mov ah,9		; Zeichen in AL, Attribute in BL ausgeben
			mov al,'Q'		; auszugebendes Zeichen
			mov bl,15d		; Zeichenfarbe
			mov cx,1		; Zeichen nur einmal ausgeben
			int 10h
			mov ah,3		; aktuelle Spalte nach DL, Zeile nach DH
			int 10h
			inc dl			; ein Zeichen weiter
			mov ah,2		; neue Curser-Position setzen
			int 10h
			mov ah,9		; Zeichen in AL, Attribute in BL ausgeben
			mov al,'N'		; auszugebendes Zeichen
			mov bl,15d		; Zeichenfarbe
			mov cx,1		; Zeichen nur einmal ausgeben
			int 10h
			mov ah,3		; aktuelle Spalte nach DL, Zeile nach DH
			int 10h
			inc dl			; ein Zeichen weiter
			mov ah,2		; neue Curser-Position setzen
			int 10h
			mov ah,9		; Zeichen in AL, Attribute in BL ausgeben
			mov al,'X'		; auszugebendes Zeichen
			mov bl,15d		; Zeichenfarbe
			mov cx,1		; Zeichen nur einmal ausgeben
			int 10h
LOOP_FOREVER:		jmp LOOP_FOREVER
			END START