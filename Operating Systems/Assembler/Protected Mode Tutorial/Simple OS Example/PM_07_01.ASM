;---------------------------------------------------------------------------
; PM_07_01.ASM
;---------------------------------------------------------------------------
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:    06.01.97
; Letze Änderungen: 31.01.97
;---------------------------------------------------------------------------
                        .386p

;-- CONST ------------------------------------------------------------------
    ;
    ; Selektors
    ;                   
        sNIL            equ 00h
        sDATA_ZERO      equ 08h                     ; maps first MB
        sCODE           equ 10h                     ; 16 Bit Code
        sDATA           equ 18h                     ; 16 Bit Data
        sSTACK          equ 20h                     ; 16 Bit Stack
        sVRAM           equ 28h                     ; Textmode VRAM
    
        sTASK0          equ 30h                     ; Task0 Segments
        sTASK0_CODE     equ 38h
        sTASK0_DATA     equ 40h
        sTASK0_STACK    equ 48h

        sHANDLER        equ 50h                     ; Task-Handler Segments
        sHANDLER_CODE   equ 58h
        sHANDLER_DATA   equ 60h
        sHANDLER_STACK  equ 68h

        b               equ byte ptr
        w               equ word ptr
        d               equ dword ptr
        f               equ fword ptr
        o               equ offset

;-- STACK ------------------------------------------------------------------
    STACK16         SEGMENT USE16 STACK
                    db 0FFFFh dup (?)
    STACK16         ENDS

;-- STRUCTURES -------------------------------------------------------------
    TSS_STRUC       struc
        tbacklink   dw 0
                    dw 0
        tesp0       dd 0
        tss0        dw 0
                    dw 0
        tesp1       dd 0
        tss1        dw 0
                    dw 0
        tesp2       dd 0
        tss2        dw 0
                    dw 0
        tcr3        dd 0
        teip        dd 0
        teflags     dd 0
        teax        dd 0
        tecx        dd 0
        tedx        dd 0
        tebx        dd 0
        tesp        dd 0
        tebp        dd 0
        tesi        dd 0
        tedi        dd 0
        tes         dw 0
                    dw 0
        tcs         dw 0
                    dw 0
        tss         dw 0
                    dw 0
        tds         dw 0
                    dw 0
        tfs         dw 0
                    dw 0
        tgs         dw 0
                    dw 0
        tldt        dd 0
        tt          dw 0
        tiopbb      dw 0
    TSS_STRUC       ends

;-- TASK0 SEGMENTS ----------------------------------------------------------
    CODE32_0        SEGMENT USE32
    CODE32_0        ENDS

    DATA32_0        SEGMENT USE32
    DATA32_0        ENDS

    STACK32_0       SEGMENT USE32
                    db 1000h dup (?)
    STACK32_0       ENDS

;-- TASKHANDLER SEGMENTS ----------------------------------------------------
    CODE32_H        SEGMENT USE32
    CODE32_H        ENDS

    DATA32_H        SEGMENT USE32
    DATA32_H        ENDS

    STACK32_H       SEGMENT USE32
                    db 1000h dup (?)
    STACK32_H       ENDS

;-- DATEN -------------------------------------------------------------------
    DATA16          SEGMENT USE16
    ;
    ; Task Segments
    ;
    TASK0           TSS_STRUC ?
    TSS_LEN         equ $-TASK0
    TASK_HANDLER    TSS_STRUC ?
    ;
    ; General Pmode Init
    ;
    gdt_limit       dw ?                            ; Size GDT
    gdt_base        dd ?                            ; BaseAdr GDT
    idt_limit       dw ?                            ; Size IDT
    idt_base        dd ?                            ; BaseAdr IDT
    ;
    ; Global Deskriptor Table
    ;
    GDT             label byte
                    dw 0, 0, 0, 0
                    dw 0FFFFh, 0, 9200h, 004Fh      ; Datasegment  (Base 0)
                    dw 0FFFFh, ?, 9A00h, 0          ; Codesegment  (16)
                    dw 0FFFFh, ?, 9200h, 0          ; Datasegment  (16)
                    dw 0FFFFh, ?, 9200h, 0          ; Stacksegment (16)
                    dw 0FFFFh, 8000h, 920Bh, 0      ; VRAM (A0000h..AFFFFh)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (0-Task)
                    dw 0FFFFh, ?, 9A00h, 0040h      ; Codesegment  (32)
                    dw 0FFFFh, ?, 9200h, 0040h      ; Datasegment  (32)
                    dw 01000h, ?, 9200h, 0040h      ; Stacksegment (32)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (Task-Handler)
                    dw 0FFFFh, ?, 9A00h, 0040h      ; Codesegment  (32)
                    dw 0FFFFh, ?, 9200h, 0040h      ; Datasegment  (32)
                    dw 01000h, ?, 9200h, 0040h      ; Stacksegment (32)
    GDTSIZE         equ $-GDT
    ;
    ; Interrupt Deskriptor Table
    ;
    IDT             label byte
                    dw IRQ_00, sCODE, 8E00h, 0
                    dw IRQ_01, sCODE, 8E00h, 0
                    dw IRQ_02, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_04, sCODE, 8E00h, 0
                    dw IRQ_05, sCODE, 8E00h, 0
                    dw IRQ_06, sCODE, 8E00h, 0
                    dw IRQ_07, sCODE, 8E00h, 0
                    dw IRQ_08, sCODE, 8E00h, 0
                    dw IRQ_09, sCODE, 8E00h, 0
                    dw IRQ_10, sCODE, 8E00h, 0
                    dw IRQ_11, sCODE, 8E00h, 0
                    dw IRQ_12, sCODE, 8E00h, 0
                    dw IRQ_13, sCODE, 8E00h, 0
                    dw IRQ_14, sCODE, 8E00h, 0
                    dw IRQ_00, sCODE, 8E00h, 0
                    dw IRQ_16, sCODE, 8E00h, 0
                    dw IRQ_17, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0

                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0
                    dw IRQ_03, sCODE, 8E00h, 0

    ; Hardware Interrupts
                    dw       0, sHANDLER, 8500h, 0
                    dw IRQ_KEY,  sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0

                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
                    dw IRQ_NULL, sCODE, 8E00h, 0
    IDTSIZE         equ $-IDT

    DATA16          ENDS

;-- CODE --------------------------------------------------------------------
    CODE16          SEGMENT USE16 'CODE' PUBLIC
                    assume cs:CODE16, ds:DATA16, ss:STACK16

    start:
                    mov ax,DATA16
                    mov ds,ax
                    mov es,ax
    ;
    ; prepare ZERO TASK
    ;
                    mov [TASK0.tedi],0
                    mov [TASK0.tcr3],0
                    mov [TASK0.teip],o task0_init
                    mov [TASK0.tcs],sTASK0_CODE
                    mov [TASK0.teflags],0
                    mov [TASK0.teax],0
                    mov [TASK0.tecx],0
                    mov [TASK0.tedx],0
                    mov [TASK0.tebx],0
                    mov [TASK0.tss],sTASK0_STACK
                    mov [TASK0.tesp],0FF0h
                    mov [TASK0.tebp],0
                    mov [TASK0.tesi],0
                    mov [TASK0.tes],sVRAM
                    mov [TASK0.tds],sTASK0_DATA
                    mov [TASK0.tfs],sTASK0_DATA
                    mov [TASK0.tgs],sTASK0_DATA
                    mov [TASK0.tldt],sNIL
                    mov [TASK0.tt],0
    ;
    ; prepare HANDLER-Task
    ;
                    mov [TASK_HANDLER.tedi],0
                    mov [TASK_HANDLER.tcr3],0
                    mov [TASK_HANDLER.teip],o taskhandler_init
                    mov [TASK_HANDLER.tcs],sHANDLER_CODE
                    mov [TASK_HANDLER.teflags],0h
                    mov [TASK_HANDLER.teax],0
                    mov [TASK_HANDLER.tecx],0
                    mov [TASK_HANDLER.tedx],0
                    mov [TASK_HANDLER.tebx],0
                    mov [TASK_HANDLER.tss],sHANDLER_STACK
                    mov [TASK_HANDLER.tesp],0FF0h
                    mov [TASK_HANDLER.tebp],0
                    mov [TASK_HANDLER.tesi],0
                    mov [TASK_HANDLER.tes],sVRAM
                    mov [TASK_HANDLER.tds],sHANDLER_DATA
                    mov [TASK_HANDLER.tfs],sDATA_ZERO
                    mov [TASK_HANDLER.tgs],sHANDLER_DATA
                    mov [TASK_HANDLER.tldt],sNIL
                    mov [TASK_HANDLER.tt],0
    ;
    ; prepare GDT Deskriptors
    ;
                    mov ax,CODE16                   ; Switching Segments
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sCODE SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sCODE SHR 3)+4],al

                    mov ax,DATA16
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sDATA SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sDATA SHR 3)+4],al

                    mov ax,STACK16
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sSTACK SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sSTACK SHR 3)+4],al
                    
                    mov ax,DATA16                   ; Task0 INIT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TASK0
                    mov w [GDT+8*(sTASK0 SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK0 SHR 3)+4],al

                    mov ax,CODE32_0
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK0_CODE SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK0_CODE SHR 3)+4],al

                    mov ax,DATA32_0
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK0_DATA SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK0_DATA SHR 3)+4],al

                    mov ax,STACK32_0
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK0_STACK SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK0_STACK SHR 3)+4],al

                    mov ax,DATA16                   ; Task HANDLER
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TASK_HANDLER
                    mov w [GDT+8*(sHANDLER SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sHANDLER SHR 3)+4],al

                    mov ax,CODE32_H
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sHANDLER_CODE SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sHANDLER_CODE SHR 3)+4],al

                    mov ax,DATA32_H
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sHANDLER_DATA SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sHANDLER_DATA SHR 3)+4],al

                    mov ax,STACK32_H
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sHANDLER_STACK SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sHANDLER_STACK SHR 3)+4],al
    ;
    ; prepare PMODE-SWITCH
    ;
                    cli
                    mov al,011h                     ; reprogram IRQ-Controller
                    out 020h,al
                    out 0A0h,al

                    mov al,20h
                    out 021h,al
                    mov al,28h
                    out 0A1h,al

                    mov al,4
                    out 021h,al
                    mov al,2
                    out 0A1h,al

                    mov al,1
                    out 021h,al
                    out 0A1h,al
                    
                    mov ax,cs
                    mov cs:rmode_cs,ax

                    mov ax,seg GDT                  ; prepare GDT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o GDT
                    mov gdt_base,eax
                    mov ax,GDTSIZE
                    mov gdt_limit,ax
                    LGDT f gdt_limit

                    mov ax,seg IDT                  ; prepare IDT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o IDT
                    mov idt_base,eax
                    mov ax,IDTSIZE
                    mov idt_limit,ax
                    LIDT f idt_limit
    ;
    ; Switch to pmode
    ;
    switch:
                    mov eax,cr0
                    or eax,1
                    mov cr0,eax

                    db 0eah
                    dw o task0_init
                    dw sTASK0_CODE

;----------------------------------------------------------------------------
    p2r:
                    cli
                    mov ax,sDATA
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax
                    mov ss,ax
                    mov esp,0FF0h
                    
                    mov al,011h                     ; reprogram IRQ-Controller
                    out 020h,al
                    out 0A0h,al

                    mov al,08h
                    out 021h,al
                    mov al,70h
                    out 0A1h,al

                    mov al,4
                    out 021h,al
                    mov al,2
                    out 0A1h,al

                    mov al,1
                    out 021h,al
                    out 0A1h,al
    ;
    ; Reset IDT
    ;
                    mov [idt_base],0
                    mov [idt_limit],03ffh
                    LIDT f idt_limit

                    mov eax,cr0
                    and eax,NOT 1                   ; clear pm-enable bit
                    and eax,NOT 8                   ; clear task-switched bit
                    mov cr0,eax

    ; "FAR-JMP" to Realmode
                    db 0eah
                    dw o rmode
    rmode_cs        dw ?

    rmode:
                    mov ax,DATA16
                    mov ds,ax
                    mov es,ax
                    mov fs,ax
                    mov gs,ax

                    mov ax,STACK16
                    mov ss,ax
                    mov esp,0FFF0h
                    sti
    ;
    ; Back to DOS
    ;
                    mov ax,4c00h
                    int 21h

;----------------------------------------------------------------------------
    IRQ_00:
                    mov al,'0'
                    jmp IRQ_ALL
    IRQ_01:
                    mov al,'1'
                    jmp IRQ_ALL
    IRQ_02:
                    mov al,'2'
                    jmp IRQ_ALL
    IRQ_03:
                    mov al,'3'
                    jmp IRQ_ALL
    IRQ_04:
                    mov al,'4'
                    jmp IRQ_ALL
    IRQ_05:
                    mov al,'5'
                    jmp IRQ_ALL
    IRQ_06:
                    mov al,'6'
                    jmp IRQ_ALL
    IRQ_07:
                    mov al,'7'
                    jmp IRQ_ALL
    IRQ_08:
                    mov al,'8'
                    jmp IRQ_ALL
    IRQ_09:
                    mov al,'9'
                    jmp IRQ_ALL
    IRQ_10:
                    mov al,'A'
                    jmp IRQ_ALL
    IRQ_11:
                    mov al,'B'
                    jmp IRQ_ALL
    IRQ_12:
                    mov al,'C'
                    jmp IRQ_ALL
    IRQ_13:
                    mov al,'D'
                    jmp IRQ_ALL
    IRQ_14:
                    mov al,'E'
                    jmp IRQ_ALL
    IRQ_16:
                    mov al,'F'
                    jmp IRQ_ALL
    IRQ_17:
                    mov al,'!'
                    jmp IRQ_ALL
    IRQ_ALL:
                    cli
                    mov bx,sVRAM
                    mov es,bx
                    mov edi,0
                    stosb

                    pop eax
                    mov edi,160
                    call ShowHexDword

                    pop eax
                    mov edi,320
                    call ShowHexDword

                    pop eax
                    mov edi,480
                    call ShowHexDword

                    pop eax
                    mov edi,640
                    call ShowHexDword

                    cli
                    jmp p2r

    IRQ_NULL:
                    push eax
                    mov al,020h
                    out 020h,al
                    out 0A0h,al
                    pop eax
                    iretd

    IRQ_KEY:
                    cli
                    in al,[60h]

                    mov al,020h
                    out 020h,al
                    sti
                    jmp p2r

    include         toolpm.inc
    CODE16          ENDS

;-- TASK0 - EXCECUTABLE -----------------------------------------------------
    CODE32_0        SEGMENT USE32
                    assume cs:CODE32_0

    task0_init:
                    mov ax,sTASK0
                    LTR ax

                    mov ax,sTASK0_DATA
                    mov ds,ax
                    mov es,ax
                    mov gs,ax
                    mov fs,ax
                    mov ax,sTASK0_STACK
                    mov ss,ax
                    mov esp,0FF0h
                    sti

    again:
                    jmp again
    CODE32_0        ENDS

;-- TASKHANDLER - EXCECUTABLE -----------------------------------------------
    CODE32_H        SEGMENT USE32
                    assume cs:CODE32_H, ds:DATA32_H, ss:STACK32_H

    taskhandler_init:
                    cli
                    mov bx,sVRAM
                    mov es,bx
                    mov edi,0
                    inc b es:[edi]
                    
                    mov al,20h
                    out 20h,al
                    sti
                    iret
                    jmp taskhandler_init

    CODE32_H        ENDS
                    END start
