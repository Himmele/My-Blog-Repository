; Assembler Version von debug_04.c!                            (Version 0.1)
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

    ; Debug-Register 1 auf Variable 'globaltest' setzen.
                    mov ebx,0Dh
                    shl ebx,20
                    or ebx,(8+512)

                    mov eax,dr7
                    and eax,00000000000000001111110000000000b
                    or eax,ebx
                    mov dr7,eax

                    mov eax,offset ds:globaltest
                    add eax,_DS_START
                    mov dr1,eax

    ; Schreibzugriff(e) auf 'globaltest' und damit INT 1 auslösen
                    mov ecx,054321h
                    xor eax,eax

    write_loop:
                    mov globaltest,eax
                    inc eax
                    dec ecx
                    jns write_loop

    ; Debug-Register Reset
                    mov eax,dr7
                    and eax,00000000000000001111110000000000b
                    mov dr7,eax

                    xor eax,eax
                    mov dr0,eax
                    mov dr1,eax
                    mov dr2,eax
                    mov dr3,eax
                    ret
    _main           endp

    _TEXT           ends

                    end
