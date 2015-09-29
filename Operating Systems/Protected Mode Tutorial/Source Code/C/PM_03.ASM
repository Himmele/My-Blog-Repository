	.386p
	ifndef	??version
?debug	macro
	endm
	endif
	?debug	S "pm_03.c"
	?debug	T "pm_03.c"
_TEXT	segment byte public use32 'CODE'
_TEXT	ends
_DATA	segment dword public use32 'DATA'
_DATA	ends
_BSS	segment dword public use32 'BSS'
_BSS	ends
DGROUP	group	_DATA,_BSS
	assume	cs:_TEXT,ds:DGROUP
_TEXT	segment byte public use32 'CODE'
c@	label	byte
	assume	cs:_TEXT
_strlen	proc	near
   ;	
   ;	int strlen (char *s)
   ;	
@5@37:
	push	ebp
	mov	ebp,esp
	push	edi
	push	esi
	mov	esi,dword ptr [ebp+8]
   ;	
   ;	    {
   ;	    int i= 0;
   ;	
	xor	edi,edi
   ;	
   ;	
   ;	    while (s[i] != '\0') i++;
   ;	
	jmp	short @8
@7:
	inc	edi
@8:
	mov	al,byte ptr [esi+edi]
	test	al,al
	jne	short @7
   ;	
   ;	
   ;	    return i;
   ;	
	mov	eax,edi
@11:
@11@0:
	pop	esi
	pop	edi
	pop	ebp
	ret
_strlen	endp
	assume	cs:_TEXT
_print	proc	near
   ;	
   ;	void print (int x, int y, char *s, int a)
   ;	
@12@36:
	push	ebp
	mov	ebp,esp
	push	edi
	push	esi
   ;	
   ;	    {
   ;	    char *video;
   ;	    int i;
   ;	
   ;	    video= (char*) lin2rel (0xB8000+ y*160+ x*2);
   ;	
	imul	eax,dword ptr [ebp+12],160
	mov	edx,dword ptr [ebp+8]
	add	edx,edx
	add	eax,edx
	add	eax,753664
	mov	esi,eax
	sub	esi,large dword ptr _DS_START
   ;	
   ;	
   ;	    if (a == -1)
   ;	
	cmp	dword ptr [ebp+20],-1
	jne	short @18
   ;	
   ;	        {
   ;	        // keine Žnderung der Attributwerte
   ;	        for (i=0; i<strlen(s); i++)
   ;	
	xor	edi,edi
	jmp	short @16
@15:
   ;	
   ;	            {
   ;	            *(video+ 0)= s[i];
   ;	
	mov	eax,dword ptr [ebp+16]
	mov	al,byte ptr [eax+edi]
	mov	byte ptr [esi],al
   ;	
   ;	            video+= 2;
   ;	
	add	esi,2
	inc	edi
@16:
	push	dword ptr [ebp+16]
	call	near ptr _strlen
	add	esp,4
	cmp	eax,edi
	jg	short @15
   ;	
   ;	            }
   ;	        }
   ;	
	jmp	short @21
@18:
   ;	
   ;	    else
   ;	        {
   ;	        // Mit Žnderung der Attributwerte
   ;	        for (i=0; i<strlen(s); i++)
   ;	
	xor	edi,edi
	jmp	short @20
@19:
   ;	
   ;	            {
   ;	            *(video++)= s[i];
   ;	
	mov	eax,dword ptr [ebp+16]
	mov	al,byte ptr [eax+edi]
	mov	byte ptr [esi],al
	inc	esi
   ;	
   ;	            *(video++)= (char) a;
   ;	
	mov	al,byte ptr [ebp+20]
	mov	byte ptr [esi],al
	inc	esi
	inc	edi
@20:
	push	dword ptr [ebp+16]
	call	near ptr _strlen
	add	esp,4
	cmp	eax,edi
	jg	short @19
@21:
@22:
@22@0:
	pop	esi
	pop	edi
	pop	ebp
	ret
_print	endp
	assume	cs:_TEXT
_main	proc	near
   ;	
   ;	void main (void)
   ;	
@23@35:
	push	ebp
	mov	ebp,esp
   ;	
   ;		{
   ;	    print (10, 10, "Hallo Welt vom Protected Mode aus BC 4.0 !!!", 0x0a);
   ;	
	mov	eax,offset DGROUP:s@
	push	10
	push	eax
	push	10
	push	10
	call	near ptr _print
	add	esp,16
@25:
@25@0:
	pop	ebp
	ret
_main	endp
_TEXT	ends
	?debug	D "pm_03.c" 8991 43509
_s@	equ	s@
	extrn	_DS_START:dword
	public	_strlen
	public	_print
	public	_main
_DATA	segment dword public use32 'DATA'
d@	label	byte
d@w	label	word
d@d	label	dword
s@	label	byte
	db	"Hallo Welt vom Protected Mode aus BC 4.0 !!!",0
_DATA	ends
_BSS	segment dword public use32 'BSS'
b@	label	byte
b@w	label	word
b@d	label	dword
_BSS	ends
	end
