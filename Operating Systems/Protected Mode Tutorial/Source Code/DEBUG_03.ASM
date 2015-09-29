; Assembler Version von debug_03.c!                            (Version 0.1)
;
; > Zugriff auf Variable im Datensegment <
;
; (c) Copyright 1997 by Jens Hohmuth. All rights reserved.

                    .386p
    extrn           _DS_START:dword
    public          _main

    _DATA           segment para public use32 'DATA'
        globaltest  dd ?
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

    ; Debug-Register 3 auf Variable 'globaltest' setzen.
                    mov ebx,0Dh
                    shl ebx,28
                    or ebx,(128+512)

                    mov eax,dr7
                    and eax,00000000000000001111110000000000b
                    or eax,ebx
                    mov dr7,eax

                    mov eax,offset ds:globaltest
                    add eax,_DS_START
                    mov dr3,eax

    ; Schreibzugriff auf 'globaltest' und damit INT 1 auslösen
                    mov eax,012345678h
                    mov globaltest,eax
                    
    ; wird nie erreicht
                    ret
    _main           endp

    _TEXT           ends

                    end
