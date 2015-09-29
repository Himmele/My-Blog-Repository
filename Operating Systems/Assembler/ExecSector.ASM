;read a disk sector form a drive and execute it


.MODEL TINY
.CODE
			
START:	mov ax,0		; wont run under Windows
			mov es,ax		; because of the first 2 lines (NEEDED FOR BOOTING)
			mov di,4096d		; offset of buffer to store sector
			mov dl,0		; drive A:
			mov dh,0		; upper side (for High Density disks)
			mov cl,3		; sector number starting at 1
			mov ch,0		; track number starting at 0
			mov al,1		; number of sectors to read and store
			mov ah,02h		; read operation
			mov bx,di		; buffer offset is ES:BX
			int 13h			; BIOS interrupt
			jmp bx;			; jump to the buffer --> code should be executable
			END START