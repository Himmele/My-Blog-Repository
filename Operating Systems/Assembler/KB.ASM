.MODEL SMALL
.STACK 256
.DATA 

.CODE
START:	
L1:		in al,64h
			test al,2
			loopne L1
			
			mov al,0EDh
			out 60h,al
			
L2:		in al,64h
			test al,1
			loope L2
			
L3:		in al,64h
			test al,2
			loopne L3
			
			mov al,01h
			out 60h,al
			
L4:		in al,64h
			test al,1
			loope L4
			
			mov ah,4Ch
			int 21h
			
			END START