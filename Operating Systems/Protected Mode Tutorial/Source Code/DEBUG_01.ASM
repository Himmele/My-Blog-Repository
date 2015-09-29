; Assembler Version von debug_01.c!                            (Version 0.1)
;
; - nutzt den gleichen Startup-Code wie debug_01.c (c0pmode.asm)
; - muß deshalb eine Funktion _main besitzen (denn diese wird von c0pmode
;   gerufen); main muß (!) im Segment _TEXT assembliert werden!
;
; (c) Copyright 1997 by Jens Hohmuth. All rights reserved.

                    .386p
    public          _main

    _TEXT           segment para public use32 'CODE'

    ;------------------------------------------------------------------------
    ; _main wird von c0pmode gerufen und im Protected Mode ausgeführt
    ;
    ; In:   ss:esp     = Stack
    ;       cs         = Codesegmentselektor auf _TEXT
    ;       ds,es,fs,gs= Datensegmentselektor (auf Datensegment _DATA)
    ;       eax,ebx,ecx,edx,esi,edi= 0
    ;------------------------------------------------------------------------
    _main           proc

    ; Debugger-Interrupt 1 auslösen
                    int 1

    _main           endp

    _TEXT           ends

                    end
