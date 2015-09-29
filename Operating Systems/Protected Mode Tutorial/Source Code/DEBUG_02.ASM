; Assembler Version von debug_02.c!                            (Version 0.1)
; (c) Copyright 1997 by Jens Hohmuth. All rights reserved.

                    .386p
    extrn           _CS_START:dword
    public          _main

    _DATA           segment para public use32 'DATA'
    _DATA           ends

    _TEXT           segment para public use32 'CODE'
                    assume cs:_TEXT, ds:_DATA

    ;------------------------------------------------------------------------
    ; _main wird von c0pmode gerufen und im Protected Mode ausgeführt
    ;
    ; In:   ss:esp     = Stack
    ;       cs         = Codesegmentselektor auf _TEXT
    ;       ds,es,fs,gs= Datensegmentselektor (auf Datensegment _DATA)
    ;       eax,ebx,ecx,edx,esi,edi= 0
    ;------------------------------------------------------------------------
    _main           proc

    ; Debug-Register 0 auf Funktion 'test' setzen.
                    xor ebx,ebx
                    or ebx,(2+512)

                    mov eax,dr7
                    and eax,00000000000000001111110000000000b
                    or eax,ebx
                    mov dr7,eax

                    mov eax,offset cs:test
                    add eax,_CS_START
                    mov dr0,eax

                    call test

    ; wird nie erreicht
                    ret
    _main           endp

    ; Aufruf von Test führt zur Debug-Exception; DR0 wurde auf das erste
    ; Byte der Funktion gesetzt.
    test            proc

    ; allgemeine Schutzverletzung auslösen; wird nie erreicht!
                    int 13
    test            endp
    _TEXT           ends

                    end
