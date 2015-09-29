; Programmer: David Lindauer
; Date: June 14,1995
; email address: gclind01@starbase.spd.louisville.edu
;
; Get into protected mode, enable paging, do something useful, get out again
;
;
; For assembly with TASM, MASM will give 2 errors which can be fixed
; by deleting the word SMALL in front of DGROUP on the lines the errors
; occur.
;

    .386P

;---------------------------------------------------------------------
; Create a GDT entry for the initializer to use
;
;  base = base address
;  limit = segment size
;  type = segment type
;  flag = gdt initializer flags (used by program)
;---------------------------------------------------------------------
GDTENTRY    MACRO   base,limit,type,flag
    dw  flag
    dw  limit AND 0ffffh
    dw  base AND 0ffffh
    db  (base SHR 16) AND 0ffh
    dw  type OR ((limit SHR 8) AND 0f00h)
    db  base SHR 24
ENDM

;---------------------------------------------------------------------
; Various symbols used to make things more readable.
;
;---------------------------------------------------------------------

NUMGDT  = 6     ; Number of entries in GDT.

IDTSIZE = 800h
GDTSIZE = NUMGDT*8
gdtzerooffset = 1	; zero offset of 386 code/data
gdtzerooffset1 = 2	; zero offset of 8086 code/data
gdtlastentry = 4

;
; Selectors, RPL = 0, in GDT
;
; These first two are used by the protected mode program
CS386   EQU 8       ; 386 code segment selector
DS386   EQU 10h     ; 386 data segment selector
;These next two are 8086 compatible code segments, only needed
; for switching back to real mode
CS8086  EQU 18h     ; 8086 code segment selector
DS8086  EQU 20h     ; 8086 data segment selector
;
; This is used to map physical memory into linear memory.
; It depends on paging, but if paging is 'see-through' then
; the 8086 memory will show up address 0 and extend through 1MB
;
DSABS   EQU 28h     ; Absolute data segment selector

;---------------------------------------------------------------------
; Define ordering of segments.
;
;---------------------------------------------------------------------

SEG8086 SEGMENT byte USE16 public 'CODE'
SEG8086 ends

SEG386 segment para USE32 public 'CODE'
SEG386 ends


ABSDATA segment dword USE32 at 0
ABSDATA ends

;---------------------------------------------------------------------
; Define a small entry stack.  We need it to make the linker happy,
; but we aren't going to use it because I don't feel like making a GDT
; entry for it.
;
;---------------------------------------------------------------------

_stack segment  para use16 STACK 'STACK'
assume ss:_stack
	db	256 DUP (0)
_stack ends


;---------------------------------------------------------------------
; IDT and GDT are first so they will be DWORD aligned
;
;---------------------------------------------------------------------
SEG386 SEGMENT
;
; The constructed GDT
;
tGDT    db  8 DUP(-1)   ; Protected mode GDT
    db  GDTSIZE-8 DUP(0)
;
; The constructed IDT
;
; We're not going to initialize either the PICS or the IDT entries for
; 386 traps and hardware interrupts.  So if they come in, the program
; crashes
;
tIDT	db IDTSIZE DUP (0)
;
; The value to load with the lgdt reg
; 
pGDT    dw  GDTSIZE-1   ; Protected mode GDT register
gdtadr  dd  offset tGDT
;
; The value to load with the lidt command
pidt	dw IDTSIZE-1
idtadr	dd offset tIDT
;
; The base of the IDT for real-mode use
;
ridt	dw 03ffh
	dd 0
	align
;
; Assembler will assemble starting at address 0.  This can be loaded
; at an arbitrary physical address, so we maintain the value 'zero'
; which is the difference between the base of DS386 and physical 0
;
zero        dd  0   ; Offset of Protected mode from absolute zero
;
; The TIMER interrupt will increment this counter, when it hits 36
; we'll exit the program ( 2 seconds)
;
timer	dd 0	; Initial value of timer
;
; so we can have a stack in our data segment :)
;
	align
	db 1024 DUP (0)
TOS	LABEL BYTE
;
SEG386 ends

ABSDATA segment
;
; The placement of the page tables is arbitrary.  They have to be aligned
; on 4K boundaries, however.  I chose to write over memory assuming it is
; unused to make the coding easier.  However, you might need to actually
; allocate the memory somehow or you could put the page tables up in
; extended memory
;
    org 90000h
;
;
; Page directory goes here
; Each page dir entry points to a page table which maps 4MB, thus with
; 1024 page tables and the page dir you can map 4GB.  The low bit is 0
; to disable the page table entries and cause a paging fault, 1 to enable
PageDir dd 1024 DUP (?)
;
; We'll map the first 16 MB of physical memory into the first 16 MB of physical
; memory ( Each page table covers 4MB in 4096 byte segments )
PageTables dd 1024 * 4 DUP (?)
;
; We're going to map the video memory at a000:0-c000:0 to memory at
; 80000000h via paging.  We'll later need to access it with a data segment
; We can either use DS386, which we have to subtract 'zero' from the
; offset 8000000h so that segmentation will map us to the right address.
; Or we can use DSABS, which has a base address of 0 and thus
; segmentation will be see-through.
VideoPageTable dd 1024 DUP (?)

ABSDATA ends


;---------------------------------------------------------------------
; Main program entry point. The first thing we do is switch to 
; protected mode. Then we call the main control program. If and when 
; the control program returns, then we return to real mode and 
; return to our caller.
;
;---------------------------------------------------------------------

seg8086 SEGMENT
    assume cs:seg8086,ds:seg386,es:nothing,ss:nothing
bootstrap:
    cld
    cli                         ; Interrupts clear throughout

    mov bx,SEG seg386                 ; Set ES=DS=CS=seg386
    mov ds,bx
    mov es,bx
    mov ss,bx			; stack switch so I don't have to have a
    mov esp,offset seg386:TOS	; GDT entry for the MSDOS stack

    ;
    ; Fixup the GDT pointers with our current location in memory.
    ;
    mov [word ptr zero],bx
    shl [zero],4                ; Fix ZERO address
    mov ebx,[zero]              ; Load it
    mov dl,[byte ptr zero + 2]  ; Load 64K segment
    add [gdtadr],ebx		; GDT and IDT regs must have a physical value
    add [idtadr],ebx

    mov esi,offset iGDT         ; Load GDT table pointers
    mov edi,offset tGDT + 8

    sub	ecx,ecx			; now DH:BP = offset of 8086 code
    mov cx,SEG8086
    shl ecx,4
    mov bp,cx
    shr ecx,16
    mov dh,cl
    ;
    ; Create the GDT
    ;
    mov cx,NUMGDT-1
gdtinit:
    lodsw                       ; Get flags
    movsd                       ; Move GDT entry to table
    movsd
    test    ax,gdtZeroOffset    ; Adjust this entry for 386 code/data?
    jz  short x8086          ; No, continue
    add [word ptr di+2-8],bx    ; Else add in the ZERO offset
    add byte ptr [di+4-8],dl    ; to make the segment base address (seg addr 0)
				; be the physical offset of the program
    jmp nooffset
x8086:
    test    ax,gdtZeroOffset1   ; adjust for 8086 code/data
    jz  short nooffset          ; no continue
    add [word ptr di+2-8],bp    ; Else add in the ZERO offset
    add byte ptr [di+4-8],dh    ; to make the segment base address (seg addr 0)
				; be the physical offset of the program
nooffset:
    test    ax,gdtLastEntry
    jnz short endGDT
    loop    gdtinit             ; But don't overflow table
endGDT:

    ; Load the GDT descriptor
    lgdt    [fword ptr pGDT]
    ; Load the IDT descriptor
    lidt    [fword ptr pIDT]

    ;
    ; Switch to protected mode.
    ;
    mov ebx,CR0     ; Set protected mode
    inc ebx
    mov CR0,ebx

    ;
    ; Perform an inter-segment jump to our protected mode code. This 
    ; is necessary to ensure that CS is reloaded with a protected 
    ; mode selector. This jump has to be 'hand coded' as it is not 
    ; supported by the assembler because its a very dangerous thing 
    ; to do unless you know what your doing.
    ;
    ; A better way to do this is with a FAR return...
    ;
    db  066h        ; Far jump into protected mode
    db  0eah
ifdef TASM
    dw  SMALL seg386:ProtectedMode
    dw  0
else
    dd  seg386:ProtectedMode
endif
    dw  CS386        ; Here we tell it to switch to prot mode seg
seg8086 ends         ; Exit real mode code segment


seg386 SEGMENT
	assume cs:seg386,ds:seg386,es:nothing,ss:seg386
;---------------------------------------------------------------------
;
; GDT entry definitions
;
;---------------------------------------------------------------------
iGDT    LABEL   WORD
;
; First two entries: base = physical start of program.
;    limit = entire address range ( 4G)
;    These are 32 bit segments
;    They are present, memory class descriptors
;    one is a read-exec code seg, the other a read-write data seg
;
; the tag 'GDTzeroOffset' causes the startup code to set the base
; address of the segment according to where DOS loaded us.
;
    GDTENTRY    0,0fffffh,0c09Ah,gdtZeroOffset  ; 386 code
    GDTENTRY    0,0fffffh,0c092H,gdtZeroOffset  ; 386 data
;
; Next two entries: base = physical start of program
;    limit = 64K
;    These are 16-bit segments
;    One is a read-exec code seg, the other a read-write data seg
;
    GDTENTRY    0,00ffffh,9Ah,gdtZeroOffset1 ; 8086 code
    GDTENTRY    0,00ffffh,92h,gdtZeroOffset  ; 8086 data (in seg386)
;
; Next entrie: base = physical 0
;   Limit = 4G
;   It's a 32-bit segment
;   It's a read-write data segment
;   Note that zer0-offsetting is not applied
    GDTENTRY    0,0fffffh,0c092h,gdtLastEntry   ; Absolute
;---------------------------------------------------------------------
; Protected mode start point. When we get here we have to reload all 
; of the selectors to ensure we do not get any protection faults 
; when we use them.
;
; Note that we do lots of shuffling with the stack... this is ONLY
; so we can use the same stack for real mode and pmode.  A simpler
; implementation would use different stacks...
;
;---------------------------------------------------------------------
ProtectedMode   LABEL BYTE
    mov ebx,ss          ; Readjust stack for protected mode
    shl ebx,4
    movzx   esp,sp
    add esp,ebx
    mov bx,DS386        ; Protected mode stack segments
    mov ss,bx
    sub esp,ss:[zero]
    mov ds,bx           ; DS,ES = primary data segment
    mov es,bx
    mov bx,DSABS        ; FS,GS = Absolute zero data segment
    mov fs,bx
    mov gs,bx
    call initpagetables
    call initinterrupt
    ;
    ; Turn paging on
    ; This assumes linear and physical addresses for this program segment
    ; match, i.e. that paging is see-through at these addresses
    ;
    mov eax,offset PageDir ; Set the page directory base register
    mov CR3, eax
    mov eax,CR0		; Set the paging enable bit to turn paging on
    or eax,80000000h	;
    mov CR0,eax

    ;
    ; Here we are in protected mode, with a sensible environment
    ; set up in the selectors.
    ;

protection:
    call userprog
endprotection:
    cli
    ; 
    ; Turn paging off by resetting the page enable bit
    ; Again, physical and linear addresses must be the same for
    ; this segment of code
    ;
    mov eax,cr0
    and eax,7fffffffh
    mov cr0,eax

    ;
    ; Prepare to return to real mode.
    ;

    ;
    ; Reload our selectors with real mode compatible segment values.
    ;
    add esp,[zero]      ; Stack adjust
    ror esp,4
    mov ebx,DS8086      ; Real mode compatible data segments
    mov ds,ebx
    mov es,ebx
    mov fs,ebx
    mov gs,ebx
    mov ss,ebx

    ;
    ; Perform an inter-segment jump to our 16 bit protected mode 
    ; code. This is necessary to ensure that CS is reloaded with 
    ; a 16 bit segment selector. This jump has to be 'hand coded' as 
    ; it is not supported by the assembler because its a very 
    ; dangerous thing to do unless you know what your doing.
    ;
    ;  Again, a FAR ret is a better way to do this.  Oh well,
    ;
    db  0eah        ; Jump to 8086 compatible segment
ifdef TASM
    dw  SMALL seg8086:RealMode1
else
    dw  seg8086:RealMode1
endif
    dw  0
    dw  CS8086      ; Switch to real mode compatible seg
seg386 ends
seg8086 segment
	assume cs:seg8086,ds:seg386,es:nothing,ss:nothing

    ;
    ; Switch to Real mode.
    ;
RealMode1:
    mov ebx,CR0     ; Back to real mode
    dec ebx
    mov CR0,ebx

    ;
    ; Perform an inter-segment jump to our real mode code. This is
    ; necessary to ensure that CS is reloaded with a real mode 
    ; segment value. This jump has to be 'hand coded' as it is 
    ; not supported by the assembler because its a very dangerous 
    ; thing to do unless you know what your doing.
    ;
    db  0eah        ; Load CS with real mode segment value
    dw  seg8086:RealMode2
    dw  seg8086

RealMode2:
    ;
    ; Restore our segment registers to GS=FS=ES=DS=seg386
    ;
    mov bx,seg386
    mov ds,bx
    mov es,bx
    mov fs,bx
    mov gs,bx
    sub esp,10h         ; Finish adjusting stack
    mov ss,sp           ; Leave at least 256 bytes for 8086
    shr esp,28
    add sp,100h
    lidt [fword ptr ridt] ; Load the real-mode idt up

    ;
    ; Here we are back in Real mode with our segment registers set 
    ; back to something sensible.
    ;

    mov ah,04ch
    int 21h
endprog:
SEG8086 ends
SEG386 segment
	assume cs:seg386,ds:seg386,es:nothing,ss:seg386
;
; Create all page tables
; We are mapping lower 16MB on a see-through basis.  You really only have
; to make sure that the routine that turns paging on and off is see-through
; although it is convenient to leave more than that see-through so you
; can access common data.  This paging routine is going to leave the
; video memory mapped in two places; you can access it either at 0a0000h
; or at 80000000h.
;
initpagetables proc
    push ds
    push es
    push fs
    pop ds 		; Switch to the absolute data segment
    push fs
    pop es
    ;
    ; First create the page tables that will allow 'see-through'
    ; access to the lower MB
    ;
    mov edi,offset PageTables
    ; The lower three bits of each page table entry are:
    ;  bit 0 = 1 means the table is accessible
    ;  bit 2 = 1 means it is a writable page
    ;  bit 3 = 1 means it is accessable at any priviledge level
    mov eax, 16 * 1024 * 1024 + 7-4096; Physical address 16MB-4096
                                      ; + lower 3 bits set
    mov ecx, 1024 * 4	; 4 tables of 1024 entries each of which
			; allocates 4096 bytes is 16 MB
fill16mb:
    mov [edi+ecx*4-4],eax; Fill in the table entry
    sub eax,4096	; Move to offset of next lower page
    loop fill16mb
    ; 
    ; Now create the page table where we map 0a0000 to 80000000h
    ; It will be 128K long, so we only need fill in the first 32 entries
    ;
    ; Before we fill it in we are going to invalidate everything in it
    ;
    mov edi, offset videopagetable
    mov ecx,1024
    mov eax, 0fffffffeh
    rep stosd
    ;
    mov edi, offset videopagetable
    mov eax,0c0000h-4096+7 ; Top page of the address to access
    mov ecx,32
fillvideo:
    mov [edi+ecx*4-4],eax  ;Fill in the table entry
    sub eax,4096           ; Move to offset of next lower page
    loop fillvideo         ; continue till done
    ;
    ; Now create the page directory.
    ;  The first 4 entries will refer to the 4 pages which hold our
    ;  16MB see-through paging.
    ;  The 512th entry will refer to the page which relocates our video
    ; to 80000000h
    ;
    ; Before we fill it in we are going to invalidate everything in it
    ;
    mov edi, offset PageDir
    mov ecx,1024
    mov eax, 0fffffffeh
    rep stosd
    ;
    mov eax,offset PageTables
    or al,7		; Add in the page table mode bits
    mov edi,offset pagedir
    mov [edi],eax
    add eax,4096
    mov [edi+4],eax
    add eax,4096
    mov [edi+8],eax
    add eax,4096
    mov [edi+12],eax
    mov eax,offset videopagetable
    or al,7		; Add in the page table mode bits
    mov [edi+512*4],eax
    pop es
    pop ds
    ret
    
initpagetables endp
;
; This is the INT 8 timer handler
;
; This ignores the BIOS data area, so our DOS clock will lose a
; couple of seconds
;
timerinterrupt proc
    push ds
    push eax
    push DS386
    pop ds	; Make sure DS is the data segment
    inc [timer] ; Increment the timer
    mov al,20h
    out 20h,al
    pop eax
    pop ds
    iretd
timerinterrupt endp
;
; Routine hooks timer interrupt, int 8.
;
; Since the hardware interrupts collide with 386 protected-mode build in
; traps, I am wont to reprogram the PICs to move the hardware interrupts
; elsewhere while in protected mode, but that's another topic
;
initinterrupt proc
    mov edi, 8 * 8 + offset tIDT; interrupts are 8 bytes long, 
				; we are hooking int 8
    mov eax,offset timerinterrupt
    mov [edi],ax	; Start by plopping in the code offset
    shr eax,16          ;
    mov [edi + 6],ax    ; High 16 bits
    mov word ptr [edi + 2],CS386 ; Now plop in the Code segment selector
    mov byte ptr [edi + 4],0 ; Always 0 for interrupts
    mov byte ptr [edi + 5],08eh ; Signifies an interrupt gate, present selector
    ret
initinterrupt endp
;
; Protected mode user program.  At this point everything is initialized
; except for unhandled interrupts.  Display data on the screen then
; disable the keyboard while we wait around for a couple of seconds
;
userprog proc
    ; Sample program to fill the screen with '!'

    push    es          ; Load the screen up with '!'
    push    fs
    push    fs
    pop es                      ; Load ES with DSABS
    mov ax,0721h                ; Black&white '!'
;
; Uncomment this line to access at the proper address
;    mov edi,0b8000h             ; screen address
;
; The next line accesses screen memory at the 80000000h page
    mov edi,80000000h + 18000h  ; B8000 = a0000 + 18000
    mov ecx,80*24               ; 24x80 screen
    rep stosw
    pop fs
    pop es
;
; Enable interrupts and wait around a couple of seconds
;
; First, disable keyboard interrupt
; This way if a key comes in we won't crash
;
    in al,21h	; PIC interrupt control port
    or al,2	; Bit to disable keyboard interrupts
    out 21h,al	; Do disable
    sti
tloop:
    cmp [timer],36 ; Wait until timer has ticked 36 times ( two seconds)
    jc tloop
    cli
;
; reenable keyboard interrupt
;
    in al,21h	; Read PIC interrupt port
    and al,0fdh ; reenable keyboard
    out 21h,al  ; Do enable
    push    es          ; Load the screen up with '*'
    push    fs
    push    fs
    pop es                      ; Load ES with DSABS
    mov ax,072ah                ; Black&white '!'
;
; Uncomment this line to access at the proper address
;    mov edi,0b8000h             ; screen address
;
; The next line accesses screen memory at the 80000000h page
    mov edi,80000000h + 18000h  ; B8000 = a0000 + 18000
    mov ecx,80*24               ; 24x80 screen
    rep stosw
    pop fs
    pop es
    ret
;
userprog endp
seg386 ends

    end bootstrap

