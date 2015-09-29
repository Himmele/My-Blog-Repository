.MODEL TINY


.CODE
			
START:	mov ah,02h
			mov dl,'S'
			int 21h
			mov ah,02h
			mov dl,'e'
			int 21h
			mov ah,02h
			mov dl,'c'
			int 21h
			mov ah,02h
			mov dl,'t'
			int 21h
			mov ah,02h
			mov dl,'o'
			int 21h
			mov ah,02h
			mov dl,'r'
			int 21h
			mov ah,4Ch
			int 21h
			END START