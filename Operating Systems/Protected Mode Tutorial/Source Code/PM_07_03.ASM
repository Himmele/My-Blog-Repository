                        .386p
    DELAY               equ 12500

;-- CONST -------------------------------------------------------------------
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
    
        sTASK1          equ 70h                     ; Task1 Segments
        sTASK1_CODE     equ 78h
        sTASK1_DATA     equ 80h
        sTASK1_STACK    equ 88h

        sTASK2          equ 90h                     ; Task2 Segments
        sTASK2_CODE     equ 98h
        sTASK2_DATA     equ 0A0h
        sTASK2_STACK    equ 0A8h

        sTASK3          equ 0B0h                    ; Task3 Segments
        sTASK3_CODE     equ 0B8h
        sTASK3_DATA     equ 0C0h
        sTASK3_STACK    equ 0C8h

        sTASK4          equ 0D0h                    ; Task4 Segments
        sTASK4_CODE     equ 0D8h
        sTASK4_DATA     equ 0E0h
        sTASK4_STACK    equ 0E8h

        b               equ byte ptr
        w               equ word ptr
        d               equ dword ptr
        f               equ fword ptr
        o               equ offset

;-- STACK -------------------------------------------------------------------
    STACK16         SEGMENT USE16 STACK
                    db 0FFFFh dup (?)
    STACK16         ENDS

;-- STRUCTURES --------------------------------------------------------------
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

;-- TASK-HANDLER SEGMENTS ---------------------------------------------------
    CODE32_H        SEGMENT USE32
    CODE32_H        ENDS

    DATA32_H        SEGMENT USE32
    adr_TSS         dd ?
    adr_GDT         dd ?
    DATA32_H        ENDS

    STACK32_H       SEGMENT USE32
                    db 1000h dup (?)
    STACK32_H       ENDS

;-- TASK1 SEGMENTS ----------------------------------------------------------
    CODE32_1        SEGMENT USE32
    CODE32_1        ENDS

    DATA32_1        SEGMENT USE32
    DATA32_1        ENDS

    STACK32_1       SEGMENT USE32
                    db 1000h dup (?)
    STACK32_1       ENDS

;-- TASK2 SEGMENTS ----------------------------------------------------------
    CODE32_2        SEGMENT USE32
    CODE32_2        ENDS

    DATA32_2        SEGMENT USE32
    DATA32_2        ENDS

    STACK32_2       SEGMENT USE32
                    db 1000h dup (?)
    STACK32_2       ENDS

;-- TASK3 SEGMENTS ----------------------------------------------------------
    CODE32_3        SEGMENT USE32
    CODE32_3        ENDS

    DATA32_3        SEGMENT USE32
    DATA32_3        ENDS

    STACK32_3       SEGMENT USE32
                    db 1000h dup (?)
    STACK32_3       ENDS

;-- TASK4 SEGMENTS ----------------------------------------------------------
    CODE32_4        SEGMENT USE32
    CODE32_4        ENDS

    DATA32_4        SEGMENT USE32
    DATA32_4        ENDS

    STACK32_4       SEGMENT USE32
                    db 1000h dup (?)
    STACK32_4       ENDS

;-- DATA --------------------------------------------------------------------
    DATA16          SEGMENT USE16
    ;
    ; Task Segments
    ;
    TASK0           TSS_STRUC ?
    TSS_LEN         equ $-TASK0
    TASK1           TSS_STRUC ?
    TASK2           TSS_STRUC ?
    TASK3           TSS_STRUC ?
    TASK4           TSS_STRUC ?
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
                    dw 0FFFFh, 0000h, 920Ah, 0      ; VRAM (A0000h..AFFFFh)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (0-Task)
                    dw 0FFFFh, ?, 9A00h, 0040h      ; Codesegment  (32)
                    dw 0FFFFh, ?, 9200h, 0040h      ; Datasegment  (32)
                    dw 01000h, ?, 9200h, 0040h      ; Stacksegment (32)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (Task-Handler)
                    dw 0FFFFh, ?, 9A00h, 0040h      ; Codesegment  (32)
                    dw 0FFFFh, ?, 9200h, 0040h      ; Datasegment  (32)
                    dw 01000h, ?, 9200h, 0040h      ; Stacksegment (32)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (First-Task)
                    dw 0FFFFh, ?, 9A00h, 0040h      ; Codesegment  (32)
                    dw 0FFFFh, ?, 9200h, 0040h      ; Datasegment  (32)
                    dw 01000h, ?, 9200h, 0040h      ; Stacksegment (32)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (Second-Task)
                    dw 0FFFFh, ?, 9A00h, 0040h      ; Codesegment  (32)
                    dw 0FFFFh, ?, 9200h, 0040h      ; Datasegment  (32)
                    dw 01000h, ?, 9200h, 0040h      ; Stacksegment (32)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (Third-Task)
                    dw 0FFFFh, ?, 9A00h, 0040h      ; Codesegment  (32)
                    dw 0FFFFh, ?, 9200h, 0040h      ; Datasegment  (32)
                    dw 01000h, ?, 9200h, 0040h      ; Stacksegment (32)

                    dw TSS_LEN,?, 8900h, 0          ; Task Segment (Fourth-Task)
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

    pal             label byte
                    include pal.inc
    DATA16          ENDS

;-- CODE --------------------------------------------------------------------
    CODE16          SEGMENT USE16 'CODE' PUBLIC
                    assume cs:CODE16, ds:DATA16, ss:STACK16

    start:
                    mov ax,DATA16
                    mov ds,ax
                    mov es,ax

                    mov ax,13h
                    int 10h

                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov esi,o pal
                    mov ecx,3*256
                    rep outsb
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
    ; prepare FIRST TASK
    ;
                    mov [TASK1.tedi],0
                    mov [TASK1.tcr3],0
                    mov [TASK1.teip],o task1_init
                    mov [TASK1.tcs],sTASK1_CODE
                    mov [TASK1.teflags],200h
                    mov [TASK1.teax],0
                    mov [TASK1.tebx],0
                    mov [TASK1.tecx],0
                    mov [TASK1.tedx],0
                    mov [TASK1.tss],sTASK1_STACK
                    mov [TASK1.tesp],0FF0h
                    mov [TASK1.tebp],0
                    mov [TASK1.tesi],0
                    mov [TASK1.tes],sTASK1_DATA
                    mov [TASK1.tds],sTASK1_DATA
                    mov [TASK1.tfs],sTASK1_DATA
                    mov [TASK1.tgs],sTASK1_DATA
                    mov [TASK1.tldt],sNIL
                    mov [TASK1.tt],0
    ;
    ; prepare SECOND TASK
    ;
                    mov [TASK2.tedi],0
                    mov [TASK2.tcr3],0
                    mov [TASK2.teip],o task2_init
                    mov [TASK2.tcs],sTASK2_CODE
                    mov [TASK2.teflags],200h
                    mov [TASK2.teax],0
                    mov [TASK2.tecx],0
                    mov [TASK2.tedx],0
                    mov [TASK2.tebx],0
                    mov [TASK2.tss],sTASK2_STACK
                    mov [TASK2.tesp],0FF0h
                    mov [TASK2.tebp],0
                    mov [TASK2.tesi],0
                    mov [TASK2.tes],sTASK2_DATA
                    mov [TASK2.tds],sTASK2_DATA
                    mov [TASK2.tfs],sTASK2_DATA
                    mov [TASK2.tgs],sTASK2_DATA
                    mov [TASK2.tldt],sNIL
                    mov [TASK2.tt],0
    ;
    ; prepare THIRD TASK
    ;
                    mov [TASK3.tedi],0
                    mov [TASK3.tcr3],0
                    mov [TASK3.teip],o task3_init
                    mov [TASK3.tcs],sTASK3_CODE
                    mov [TASK3.teflags],200h
                    mov [TASK3.teax],0
                    mov [TASK3.tecx],0
                    mov [TASK3.tedx],0
                    mov [TASK3.tebx],0
                    mov [TASK3.tss],sTASK3_STACK
                    mov [TASK3.tesp],0FF0h
                    mov [TASK3.tebp],0
                    mov [TASK3.tesi],0
                    mov [TASK3.tes],sTASK3_DATA
                    mov [TASK3.tds],sTASK3_DATA
                    mov [TASK3.tfs],sTASK3_DATA
                    mov [TASK3.tgs],sTASK3_DATA
                    mov [TASK3.tldt],sNIL
                    mov [TASK3.tt],0
    ;
    ; prepare FOURTH TASK
    ;
                    mov [TASK4.tedi],0
                    mov [TASK4.tcr3],0
                    mov [TASK4.teip],o task4_init
                    mov [TASK4.tcs],sTASK4_CODE
                    mov [TASK4.teflags],200h
                    mov [TASK4.teax],0
                    mov [TASK4.tecx],0
                    mov [TASK4.tedx],0
                    mov [TASK4.tebx],0
                    mov [TASK4.tss],sTASK4_STACK
                    mov [TASK4.tesp],0FF0h
                    mov [TASK4.tebp],0
                    mov [TASK4.tesi],0
                    mov [TASK4.tes],sTASK4_DATA
                    mov [TASK4.tds],sTASK4_DATA
                    mov [TASK4.tfs],sTASK4_DATA
                    mov [TASK4.tgs],sTASK4_DATA
                    mov [TASK4.tldt],sNIL
                    mov [TASK4.tt],0
    ;
    ; prepare HANDLER
    ;
                    mov [TASK_HANDLER.tedi],0
                    mov [TASK_HANDLER.tcr3],0
                    mov [TASK_HANDLER.teip],o taskhandler_init
                    mov [TASK_HANDLER.tcs],sHANDLER_CODE
                    mov [TASK_HANDLER.teflags],200h
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

                    mov ax,DATA32_H
                    mov fs,ax
                    assume fs:DATA32_H

                    mov ax,DATA16
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TASK_HANDLER
                    mov fs:[adr_TSS],eax

                    mov ax,DATA16
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o GDT
                    mov fs:[adr_GDT],eax
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

                    mov ax,DATA16                   ; Task1 INIT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TASK1
                    mov w [GDT+8*(sTASK1 SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK1 SHR 3)+4],al

                    mov ax,CODE32_1
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK1_CODE SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK1_CODE SHR 3)+4],al

                    mov ax,DATA32_1
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK1_DATA SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK1_DATA SHR 3)+4],al

                    mov ax,STACK32_1
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK1_STACK SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK1_STACK SHR 3)+4],al

                    mov ax,DATA16                   ; Task2 INIT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TASK2
                    mov w [GDT+8*(sTASK2 SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK2 SHR 3)+4],al

                    mov ax,CODE32_2
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK2_CODE SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK2_CODE SHR 3)+4],al

                    mov ax,DATA32_2
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK2_DATA SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK2_DATA SHR 3)+4],al

                    mov ax,STACK32_2
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK2_STACK SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK2_STACK SHR 3)+4],al

                    mov ax,DATA16                   ; Task3 INIT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TASK3
                    mov w [GDT+8*(sTASK3 SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK3 SHR 3)+4],al

                    mov ax,CODE32_3
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK3_CODE SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK3_CODE SHR 3)+4],al

                    mov ax,DATA32_3
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK3_DATA SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK3_DATA SHR 3)+4],al

                    mov ax,STACK32_3
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK3_STACK SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK3_STACK SHR 3)+4],al

                    mov ax,DATA16                   ; Task4 INIT
                    and eax,0FFFFh
                    shl eax,4
                    add eax,o TASK4
                    mov w [GDT+8*(sTASK4 SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK4 SHR 3)+4],al

                    mov ax,CODE32_4
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK4_CODE SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK4_CODE SHR 3)+4],al

                    mov ax,DATA32_4
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK4_DATA SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK4_DATA SHR 3)+4],al

                    mov ax,STACK32_4
                    and eax,0FFFFh
                    shl eax,4
                    mov w [GDT+8*(sTASK4_STACK SHR 3)+2],ax
                    shr eax,16
                    mov b [GDT+8*(sTASK4_STACK SHR 3)+4],al

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
    ;
    ; Timer Init
    ;
                    mov al,00110110b
                    out 43h,al
                    mov ax,DELAY
                    out 40h,al
                    mov al,ah
                    out 40h,al

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
    ;
    ; Timer Reset
    ;
                    mov al,00110110b
                    out 43h,al
                    xor ax,ax
                    out 40h,al
                    mov al,ah
                    out 40h,al

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
                    mov ax,03h
                    int 10h

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
                    in al,[60h]
					cmp al,1
					jne EXIT_OS
					
	EXIT_OS:
					cli
                    
                    mov dx,03c8h
                    mov al,0
                    out dx,al
                    inc dx
                    mov al,63
                    out dx,al
                    mov al,0
                    out dx,al
                    out dx,al

                    mov al,020h
                    out 020h,al
                    jmp p2r

    include         toolpm.inc
    CODE16          ENDS

;-- TASK0 - EXECUTABLE ------------------------------------------------------
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
                    sti

    again:
                    jmp again
    CODE32_0        ENDS

;-- TASK1 - EXECUTABLE (FIRE) -----------------------------------------------
    ; constants
    XSIZE           equ 160
    YSIZE           equ 100

    ; code
    CODE32_1        SEGMENT USE32
                    assume cs:CODE32_1, ds:DATA32_1, ss:STACK32_1

    task1_init:
                    call line_scroll
                    call line_new
                    call show

                    mov eax,zp
                    xchg eax,qp
                    mov zp,eax
                    jmp task1_init

    line_new        proc
    ;
    ; Create 80 Random Dots
    ;
                    xor ecx,ecx

    xloop:
                    mov edi,zp
                    add edi,XSIZE*(YSIZE-10)
                    add edi,ecx

                    mov bl,6
                    call random
                    and eax,31
                    add eax,20

                    stosb
                    add edi,XSIZE-1
                    stosb
                    add edi,XSIZE-1
                    stosb
                    add edi,XSIZE-1

                    inc ecx
                    cmp ecx,XSIZE-1
                    jne xloop
    ;
    ; Create Random Number of BIG DOTS
    ;
                    mov bl,5
                    call random
                    and eax,7
                    inc eax

                    mov ecx,eax

    xloop2:
                    mov bl,8
                    call random
                    and eax,255
                
                    cmp eax,XSIZE-1
                    jb valueOK
                    sub eax,XSIZE-1

    valueOK:
                    mov edi,zp
                    add edi,XSIZE*(YSIZE-12)
                    add edi,eax

                    mov al,63
                    stosb
                    stosb
                    stosb
                    add edi,XSIZE-3

                    stosb
                    stosb
                    stosb
                    add edi,XSIZE-3

                    stosb
                    stosb
                    stosb
                    dec ecx
                    jnz xloop2
                    ret
                    endp

    line_scroll     proc
                    mov esi,qp
                    add esi,XSIZE
                    mov edi,zp

                    mov ecx,XSIZE*(YSIZE-10)
                    xor ebx,ebx

    loop1:
                    xor ax,ax
                    mov al,[esi-XSIZE+1]
                    mov bl,[esi-XSIZE]
                    add ax,bx
                    mov bl,[esi-XSIZE-1]
                    add ax,bx
                    mov bl,[esi-1]
                    add ax,bx
                    mov bl,[esi+1]
                    add ax,bx
                    mov bl,[esi+XSIZE-1]
                    add ax,bx
                    mov bl,[esi+XSIZE]
                    add ax,bx
                    mov bl,[esi+XSIZE+1]
                    add ax,bx
                    shr ax,3
        
                    or ax,ax
                    je null
;                    dec al

    null:
                    stosb
                    inc esi
                    dec ecx
                    jnz loop1
                    ret
                    endp

    show            proc
                    mov esi,zp
                    mov edi,o task1_vram
                    add edi,XSIZE*10

                    mov ecx,XSIZE*(YSIZE-10) / 4
                    rep movsd
                    ret
                    endp

    ;------------------------------------------------------------------------
    ; RANDOM (ROUTINE BY ADAM SEYCHELL, BASED UPON AN ALGORITHMUS FROM AN
    ;         ARTICLE IN DOCTOR DOBBS JOURNAL ISSUE MAY 1991,
    ;         ADDITIONAL CODING BY JENS HOHMUTH IN 1995)
    ;
    ; In :  bl - Bit size
    ; Out: eax - Random Number
    ; Chg: nil ?
    ;------------------------------------------------------------------------
    random          proc near
                    push ebx ecx edx
                    mov cl,bl

                    xor eax,eax
                    cmp eax,random_init
                    je Initalize_Random_number

        J75:
                    mov bl,byte ptr random_init
                    and bl,1

        Gen_bit:                                ; make n bit numbers
                    shl eax,1
                    mov edx,random_init

                    shr edx,9
                    xor bl,dl

                    shr edx,5
                    xor bl,dl

                    bt ebx,1
                    rcr dword ptr random_init,1
                    setc bl
                    or  al,bl
    
                    dec cl
                    jnz Gen_bit
                    pop edx ecx ebx
                    ret

        Initalize_Random_number:            ; Get inital random number from CMOS time
                    push es
                    mov ax,sDATA_ZERO
                    mov es,ax

                    mov al,0                    ; Get seconds
                    out 70h,al
                    in al,71h
                    shl eax,8
                    mov al,2                    ; Get minute
                    out 70h,al
                    in al,71h
                    shl eax,8
                    xor eax,es:[046Ch]          ; throw in number of ticks
                    pop es
                    not eax
                    mov random_init,eax
                    jmp J75
                    endp

    CODE32_1        ENDS

    DATA32_1        SEGMENT USE32
    random_init     dd 0
                    db XSIZE dup (0)
    zbuffer         db XSIZE*YSIZE dup (0)
                    db XSIZE dup (0)
    qbuffer         db XSIZE*YSIZE dup (0)
    zp              dd zbuffer
    qp              dd qbuffer

    task1_vram      db 100*160 dup (0)
    DATA32_1        ENDS


;-- TASK2 - EXECUTABLE (PLASMA) ---------------------------------------------
    ; constants
    max             equ 2047
    XSIZE_2         equ 160
    YSIZE_2         equ 100

    ; code
    CODE32_2        SEGMENT USE32
                    assume cs:CODE32_2, ds:DATA32_2, ss:STACK32_2

    task2_init:
                    xor eax,eax
                    xor ebx,ebx
                    xor ecx,ecx

    repeat:
                    mov ax,v1
                    add ax,2
                    and ax,max
                    mov v1,ax
                    mov b1,ax

                    mov ax,v2
                    add ax,max-4
                    and ax,max
                    mov v2,ax
                    mov b2,ax

                    mov ax,v3
                    add ax,3
                    and ax,max
                    mov v3,ax
                    mov b3,ax

                    mov edi,o task2_vram
                    mov ebp,YSIZE_2

    loopline:
                    add b1,17
                    add b2,max-1
                    add b3,6

                    and b1,max
                    and b2,max
                    and b3,max
    ;
    ; eine (!) Plasma Linie
    ;
                    mov dx,b1
                    mov bx,b2
                    mov cx,b3

                    mov esi,XSIZE_2

    loopthis:
                    mov al,b [sinus+edx*2]
                    add al,b [sinus+ebx*2]
                    add al,b [sinus+ecx*2]
                    and al,63
                    add al,64
                    add edx,max-3
                    add ebx,23
                    add ecx,9
                    and edx,max
                    and ebx,max
                    and ecx,max
                    stosb

                    dec esi
                    jnz loopthis

                    dec ebp
                    jnz loopline
                    jmp task2_init
    CODE32_2        ENDS

    DATA32_2        SEGMENT USE32
    sinus           label word
                    include sintab.inc
    v1              dw 0
    v2              dw 0
    v3              dw 0
    b1              dw ?
    b2              dw ?
    b3              dw ?

    task2_vram      db 100*160 dup (1)
    DATA32_2        ENDS

;-- TASK3 - EXECUTABLE (ZOOM) -----------------------------------------------
    ; constants
    XSIZE_3         equ 160
    YSIZE_3         equ 100

    ; code
    CODE32_3        SEGMENT USE32
                    assume cs:CODE32_3, ds:DATA32_3, ss:STACK32_3

    task3_init:
                    cmp task3_ready,0
                    jne task3_init

                    mov edi,o task3_vram            ; Adjust Dest. and
                    mov esi,o pic+4+3*256           ; Source

                    mov edx,YSIZE_3                 ; Scanline Counter
                    xor eax,eax                     ; current Fix Point

    Looping:
                    and ebp,511
                    add eax,Tab [ebp*4]
                    rol eax,16
                    or ax,ax
                    je old

                    mov ebx,eax
                    and ebx,0ffffh
                    imul ebx,XSIZE_3
                    add esi,ebx
                    xor ax,ax

    old:
                    rol eax,16

                    push esi
                    mov ecx,XSIZE_3 / 4
                    rep movsd
                    pop esi

                    inc ebp
                    dec edx
                    jnz Looping

                    sub ebp,YSIZE_3-1
                    mov task3_ready,1
                    jmp task3_init
    CODE32_3        ENDS

    DATA32_3        SEGMENT USE32
    Tab             label dword
                    include sintab2.inc
    pic             label byte
                    include face.inc
    task3_vram      db 100*160 dup (100)
    task3_ready     db ?
    DATA32_3        ENDS

;-- TASK4 - EXECUTABLE ( sprite ) -------------------------------------------
    ; constants
    XSIZE_4         equ 160
    YSIZE_4         equ 100

    ; code
    CODE32_4        SEGMENT USE32
                    assume cs:CODE32_4, ds:DATA32_4, ss:STACK32_4

    ;- PUTSPRITE ------------------------------------------------------------
    ; Put's a Sprite on the Screen
    ;
    ; In : eax/ebx - X/Y Start Punkt
    ;      esi     - Offset to Sprite Data
    ; Out: nil
    ;------------------------------------------------------------------------
    PutSprite       proc
                    imul ebx,XSIZE_4
                    add ebx,eax
                    mov edi,ebx
                    add edi,o task4_vram

                    mov edx,XSIZE_4
                    xor eax,eax
                    lodsw                           ; Get XSize
                    mov ebx,eax                     ; ebx= XSize
                    sub edx,eax

                    xor eax,eax
                    lodsw                           ; Get YSize
                    mov ebp,eax                     ; ebp= YSize

    RowLoop:
                    mov ecx,ebx

    ; Copy one Line
                    shr ecx,1                       ; ecx / 2
                    jnc Lab1                        ; ungerade ?
                    movsb                           ; ja, Byte 1 schreiben

    Lab1:
                    jcxz NextRow
                    shr ecx,1
                    jnc Lab2
                    movsw

    Lab2:
                    rep movsd

    NextRow:
                    add edi,edx
                    dec ebp
                    jnz RowLoop
                    ret
                    endp

    ;- CLEARSPRITE ----------------------------------------------------------
    ; Clears Sprite on the Screen
    ;
    ; In : eax/ebx - X/Y Start Punkt
    ;      esi     - Offset to Sprite Data
    ; Out: nil
    ;------------------------------------------------------------------------
    ClearSprite     proc
                    imul ebx,XSIZE_4
                    add ebx,eax
                    mov edi,ebx
                    add edi,o task4_vram

                    mov edx,XSIZE_4
                    xor eax,eax
                    lodsw                           ; Get XSize
                    mov ebx,eax                     ; Save in ebx
                    sub edx,eax
                    xor eax,eax
                    lodsw                           ; Get YSize
                    mov ebp,eax
                    mov eax,200
                    imul eax,01010101h

    RowLoop2:
                    mov ecx,ebx

    ; Clear a Line
                    shr ecx,1                       ; ecx / 2
                    jnc Lab5                        ; ungerade ?
                    stosb                           ; ja, Byte 1 schreiben

    Lab5:
                    jcxz NextRow2
                    shr ecx,1
                    jnc Lab6
                    stosw

    Lab6:
                    rep stosd

    NextRow2:
                    add edi,edx
                    dec ebp
                    jnz RowLoop2
                    ret
                    endp

    task4_init:
                    mov x,50
                    mov y,50
                    mov xa,1
                    mov ya,-1

    LoopIt:
                    cmp task4_ready,0
                    jne LoopIt

    ; CLEAR SPRITE
                    mov eax,x
                    mov ebx,y
                    mov esi,o MySprite
                    call ClearSprite

    ; Calc new X
                    mov eax,x
                    add eax,xa

                    cmp eax,XSIZE_4-32
                    jl ok1
                    mov xa,-4

    ok1:
                    or eax,eax
                    jg ok2
                    mov xa,2

    ok2:
                    mov x,eax

    ; Calc new Y
                    mov eax,y
                    add eax,ya

                    cmp eax,YSIZE_4-32
                    jl ok3
                    mov ya,-2

    ok3:
                    or eax,eax
                    jg ok4
                    mov ya,5

    ok4:
                    mov y,eax

    ; PUT SPRITE
                    mov eax,x
                    mov ebx,y
                    mov esi,o MySprite
                    call PutSprite

                    mov task4_ready,1
                    jmp LoopIt
    CODE32_4        ENDS

    DATA32_4        SEGMENT USE32
    MySprite        label dword
                    include jimmy.inc

    x               dd 0
    y               dd 0
    xa              dd 0
    ya              dd 0
    task4_ready     db 0
    task4_vram      db 100*160 dup (200)
    DATA32_4        ENDS

;-- TASK_HANDLER EXECUTABLES ------------------------------------------------
    CODE32_H        SEGMENT USE32
                    assume cs:CODE32_H, ds:DATA32_H, ss:STACK32_H

    taskhandler_init:
                    cld

    ; TASK 1
                    cli
                    mov al,20h
                    out 20h,al
                    sti
                    db 0eah
                    dd 0
                    dw sTASK1

                    cli
                    mov edi,0
                    mov ax,sTASK1_DATA
                    mov ds,ax
                    mov esi,o task1_vram
                    mov edx,100
    line_loop1:     mov ecx,160 / 4
                    rep movsd
                    add edi,160
                    dec edx
                    jnz line_loop1

    ; TASK 2
                    mov al,20h
                    out 20h,al
                    sti
                    db 0eah
                    dd 0
                    dw sTASK2

                    cli
                    mov edi,100*320+160
                    mov ax,sTASK2_DATA
                    mov ds,ax
                    mov esi,o task2_vram
                    mov edx,100
    line_loop2:     mov ecx,160 / 4
                    rep movsd
                    add edi,160
                    dec edx
                    jnz line_loop2

    ; TASK3
                    mov al,20h
                    out 20h,al
                    sti
                    db 0eah
                    dd 0
                    dw sTASK3

                    cli
                    mov ax,sTASK3_DATA
                    mov ds,ax
                    assume ds:DATA32_3
                    cmp task3_ready,1
                    jne short next_task4
                    mov task3_ready,0

                    mov edi,160
                    mov esi,o task3_vram
                    mov edx,100
    line_loop3:     mov ecx,160 / 4
                    rep movsd
                    add edi,160
                    dec edx
                    jnz line_loop3

    next_task4:
                    assume ds:DATA32_H

    ; TASK 4
                    mov al,20h
                    out 20h,al
                    sti
                    db 0eah
                    dd 0
                    dw sTASK4

                    cli
                    mov edi,100*320
                    mov ax,sTASK4_DATA
                    mov ds,ax
                    assume ds:DATA32_4
                    cmp task4_ready,1
                    jne short next_task5
                    mov task4_ready,0

                    mov esi,o task4_vram
                    mov edx,100
    line_loop4:     mov ecx,160 / 4
                    rep movsd
                    add edi,160
                    dec edx
                    jnz line_loop4

    next_task5:
                    assume ds:DATA32_H
                    
                    mov ax,sHANDLER_DATA
                    mov ds,ax

                    mov ebx,adr_TSS
                    mov w fs:[ebx.tbacklink],sTASK0

                    mov ecx,adr_GDT
                    mov b fs:[ecx+8*(sTASK1 SHR 3)+5],89h
                    mov b fs:[ecx+8*(sTASK2 SHR 3)+5],89h
                    mov b fs:[ecx+8*(sTASK3 SHR 3)+5],89h
                    mov b fs:[ecx+8*(sTASK4 SHR 3)+5],89h

                    mov al,20h
                    out 20h,al
                    sti
                    iret
                    jmp taskhandler_init

    CODE32_H        ENDS
                    END start
