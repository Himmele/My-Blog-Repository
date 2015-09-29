;--------------------------------------------------------------------
; XMS.ASM - Demonstration der Nutzung des Extended Memory
;
; Die hier definierten Assembleroutinen sind für die Verwendung mit
; einem C-Compiler (mit einem FAR-Speichermodell, z.B. LARGE)
; gedacht. Bei Verwendung anderer Speichermodelle oder Compiler
; müssen wahrscheinlich Änderungen vorgenommen werden.
;
; ASSEMBLIEREN: TASM /ML XMS.ASM
;
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:     09.10.96
; Letze Aenderungen: 10.01.97
;--------------------------------------------------------------------
                .model large, c
                .stack 100h

    public      xms_check,              \
                xms_version,            \
                xms_getfreemem,         \
                xms_alloc,              \
                xms_free,               \
                xms_lock,               \
                xms_unlock,             \
                xms_ram2xms,            \
                xms_xms2ram,            \
                xms_xms2xms,            \
                xms_errormsg

                .data
    TRUE        equ 1
    FALSE       equ 0
    b           equ byte ptr
    w           equ word ptr

    ; Kopierstruktur für Funktion 0Bh
    XMS_COPY    label word
        csize   dd ?
        qhandle dw ?
        qoff    dd ?
        zhandle dw ?
        zoff    dd ?

    ; Einsprungadresse in XMS-Handler, wird von xms_init initialisiert
    XMS_ADR     dd ?

    ; Offsettabelle für xms_errormsg Funktion
    XMS_ERROR   db ?
    XMS_MSGTAB  dw error80, error81, error82
                dw nomsg, nomsg, nomsg, nomsg, nomsg, nomsg, nomsg, nomsg
                dw nomsg, nomsg, nomsg
                dw error8E, error8F
                dw error90, error91, error92, error93, error94
                dw nomsg, nomsg, nomsg, nomsg, nomsg, nomsg, nomsg, nomsg
                dw nomsg, nomsg, nomsg
                dw errorA0, errorA1, errorA2, errorA3, errorA4, errorA5, errorA6
                dw errorA7, errorA8, errorA9, errorAA, errorAB, errorAC, errorAD
                dw nomsg, nomsg
                dw errorB0, errorB1, errorB2
                dw nomsg
    error80     db "Funktion ist nicht bekannt !                                                   ", 0
    error81     db "VDISK-Ramdisk gefunden !                                                       ", 0
    error82     db "Fehler auf der A20 Adressleitung !                                             ", 0
    error8E     db "allgemeiner Treiberfehler !                                                    ", 0
    error8F     db "irreparabler Fehler !                                                          ", 0
    error90     db "HMA nicht vorhanden !                                                          ", 0
    error91     db "HMA schon vergeben !                                                           ", 0
    error92     db "die in DX angegebene Speichergroesse ist zu klein !                            ", 0
    error93     db "HMA nicht allokiert !                                                          ", 0
    error94     db "A20 Adressleitung ist noch an !                                                ", 0
    errorA0     db "kein XMS mehr verfuegbar !                                                     ", 0
    errorA1     db "alle XMS-Handles sind belegt !                                                 ", 0
    errorA2     db "Handle ist ungueltig !                                                         ", 0
    errorA3     db "Quell-Handle ist ungueltig !                                                   ", 0
    errorA4     db "Quell-Offset ist ungueltig !                                                   ", 0
    errorA5     db "Ziel-Handle ist ungueltig !                                                    ", 0
    errorA6     db "Ziel-Offset ist ungueltig !                                                    ", 0
    errorA7     db "ungueltige Laenge fuer Move-Funktion !                                         ", 0
    errorA8     db "unzulaessige Ueberlappung bei Move-Funktion !                                  ", 0
    errorA9     db "Parity-Error !                                                                 ", 0
    errorAA     db "UMB nicht gesperrt !                                                           ", 0
    errorAB     db "UMB noch gesperrt !                                                            ", 0
    errorAC     db "Ueberlauf des UMB-Sperrzaehlers !                                              ", 0
    errorAD     db "UMB kann nicht gesperrt werden !                                               ", 0
    errorB0     db "kleinerer UMB verfuegbar !                                                     ", 0
    errorB1     db "Kein UMB mehr verfuegbar !                                                     ", 0
    errorB2     db "Ungueltige UMB-Segmentadresse !                                                ", 0
    nomsg       db "Ungueltige Fehlernummer !                                                      ", 0

                .code
                .386
    ;----------------------------------------------------------------
    ; int xms_check (void);
    ;
    ; Versucht über Interrupt 2Fh einen XMS-Treiber zu identifizieren.
    ; falls XMS-Treiber verfügbar ist, speichert sie zusätzlich die XMS-
    ; Einsprungadresse in der Variablen: XMS_ADR
    ;
    ; In:       void
    ; Out:      TRUE  (=1), wenn erfolgreich (XMS-Treiber vorhanden)
    ;           FALSE (=0), wenn kein XMS-Treiber gefunden werden konnte
    ;----------------------------------------------------------------
    xms_check   proc
                push ds es

    ; XMS-Treiber über Interrupt 2Fh erkennen
                mov ax,4300h
                int 2fh

    ; al  = 80h Treiber installiert
    ; al <> 80h Treiber nicht vorhanden
                cmp al,80h
                jne check_quit_false

    ; Treiber vorhanden, Einsprungadresse ermitteln
                mov ax,4310h
                int 2fh

    ; es:bx = Einsprungsadresse, unter XMS_ADR im Datensegent speichern
                mov ax,@data
                mov ds,ax

                mov w [XMS_ADR+0],bx
                mov w [XMS_ADR+2],es

    ; Funktion mit Erfolgsmeldung verlassen
                mov ax,TRUE
                jmp check_quit

    check_quit_false:
                mov ax,FALSE

    check_quit:
                pop es ds
                ret
                endp

    ;----------------------------------------------------------------
    ; void xms_version (int far *version);
    ;
    ; ermittelt die XMS-Versionsnummer und speichert sie unter der
    ; übergebenen Adresse
    ;
    ; In:       version - (far) Zeiger auf Word Speicherplatz
    ; Out:      void
    ;----------------------------------------------------------------
    xms_version proc ver:dword
                push es di ds 
                mov ax,@data
                mov ds,ax

    ; Funktion 00: XMS-Treiber Version ermitteln
                mov ah,00h
                call [dword ptr XMS_ADR]

                les di,ver
                stosw
                pop ds di es
                ret
                endp

    ;----------------------------------------------------------------
    ; void xms_getfreemem (int far *biggest, int far *all);
    ;
    ; ermittelt den noch freien Extended Memory !
    ; Die Funktion liefert dazu über den ersten Paramter den größten
    ; noch zur Verfügung stehenden Block und über den zweiten Paramter
    ; die Gesamtgröße des freien Extended Memory in KByte
    ;
    ; In:       biggest - (far) Zeiger auf Word Speicherplatz
    ;                   - enthält nach dem Aufruf die Länge des größten
    ;                     zur Verfügung stehenden XMS-Block
    ;           all     - (far) Zeiger auf Word Speicherplatz
    ;                   - Gesamtgröße noch verfügbaren XMS-Speichers
    ; Out:      void
    ;----------------------------------------------------------------
    xms_getfreemem proc biggest:dword, all:dword
                push ds es di
                mov ax,@data
                mov ds,ax

    ; Funktion 08h: Freies XMS ermitteln
                mov ah,08h
                call [dword ptr XMS_ADR]

                les di,biggest
                stosw

                les di,all
                mov ax,dx
                stosw

                pop di es ds
                ret
                endp

    ;----------------------------------------------------------------
    ; int xms_alloc (int far *handle, int size);
    ;
    ; reserviert <size> KBytes im Extended Memory und speichert das
    ; zurückgelieferte Handle unter *handle
    ;
    ; Im Fehlerfall liefert die Routine FALSE (Fehlercode ueber: xms_errormsg
    ; erreichbar), sonst TRUE.
    ;
    ; In:       handle - (far) Zeiger auf Word Speicherplatz
    ;                  - nimmt Handle auf
    ;           size   - Größe des Angeforderten Bereiches (KB)
    ; Out:      TRUE   - Funktion erfolgreich
    ;           FALSE  - Fehler aufgetreten
    ;----------------------------------------------------------------
    xms_alloc   proc handle:dword, size:word
                push ds es di
                mov ax,@data
                mov ds,ax

    ; Funktion 09h: XMS-Speicher allokieren
                mov ah,09h
                mov dx,size
                call [dword ptr XMS_ADR]

                cmp ax,0
                je alloc_quit_false

    ; Funktion fehlerfrei
    ; dx = handle
                les di,handle

                mov ax,dx
                stosw

                mov ax,TRUE
                jmp alloc_quit

    alloc_quit_false:
                mov XMS_ERROR,bl
                mov ax,FALSE

    alloc_quit:
                pop di es ds
                ret
                endp

    ;----------------------------------------------------------------
    ; int xms_free (int handle);
    ;
    ; gibt durch funktion xms_alloc reservierten XMS-Speicher wieder frei
    ;
    ; Im Fehlerfall liefert die Routine FALSE (Fehlercode ueber: xms_errormsg
    ; erreichbar), sonst TRUE.
    ;
    ; In:       handle - Handle des freizugebenen Speichers
    ; Out:      TRUE   - Funktion erfolgreich
    ;           FALSE  - Fehler aufgetreten
    ;----------------------------------------------------------------
    xms_free    proc handle:word
                push ds
                mov ax,@data
                mov ds,ax

    ; Funktion 0Ah: XMS-Speicher freigeben
                mov ah,0Ah
                mov dx,handle
                call [dword ptr XMS_ADR]

                cmp ax,0
                je free_quit_false

    ; Funktion fehlerfrei
                mov ax,TRUE
                jmp free_quit

    free_quit_false:
                mov XMS_ERROR,bl
                mov ax,FALSE

    free_quit:
                pop ds
                ret
                endp

    ;----------------------------------------------------------------
    ; int xms_ram2xms (void *quelle,
    ;                  int zielhandle, long zieloffset,
    ;                  long size);
    ;
    ; kopiert den durch den Zeiger <quelle> angegebenen, <size> Byte langen
    ; Bereich, in den durch das <zielhandle> und <zieloffset> angegebenen
    ; Bereich im Extended Memory
    ;
    ; Im Fehlerfall liefert die Routine FALSE (Fehlercode über: xms_errormsg
    ; erreichbar), sonst TRUE.
    ;
    ; In:  quelle     - (far) Zeiger auf Quellbereich (im 1 MB Adressraum)
    ;      zielhandle - das Handle, eines durch Fkt. 09h allokierten EMB
    ;      zieloffset - das Offset innerhalb des EMB
    ;      size       - die Länge des zu kopierenden Bereichs
    ; Out: TRUE       - Funktion erfolgreich
    ;      FALSE      - Fehler aufgetreten
    ;----------------------------------------------------------------
    xms_ram2xms proc quelle:dword,                          \
                     zielhandle:word, zieloffset:dword,     \
                     size:dword

                push ds si es di

    ; "Kopier-Struktur" mit Werten füllen
                mov ax,@data
                mov es,ax
                mov di,offset XMS_COPY

    ; Länge des zu kopierenden Bereiches
                mov ax,w [size+0]
                stosw
                mov ax,w [size+2]
                stosw

    ; Quell Handle (hier: 0)
                mov ax,0
                stosw

    ; Quell-"Offset" setzen
                mov ax,w [quelle+0]
                stosw
                mov ax,w [quelle+2]
                stosw

    ; Ziel-"Handle"
                mov ax,zielhandle
                stosw

    ; Ziel-"Offset"
                mov ax,w [zieloffset+0]
                stosw
                mov ax,w [zieloffset+2]
                stosw

    ; Funktion 0Bh aufrufen
                mov ax,@data
                mov ds,ax

                mov ah,0Bh
                mov si,offset XMS_COPY
                call [dword ptr XMS_ADR]

                cmp ax,0
                je ram2xms_quit_false

    ; Funktion fehlerfrei
                mov ax,TRUE
                jmp ram2xms_quit

    ram2xms_quit_false:
                mov XMS_ERROR,bl
                mov ax,FALSE

    ram2xms_quit:
                pop di es si ds
                ret
                endp

    ;----------------------------------------------------------------
    ; int xms_xms2ram (int quellhandle, long quelloffset,
    ;                  void *ziel,
    ;                  long size);
    ;
    ; kopiert den durch das Handle <quellhandle> und <quelloffset>, <size>
    ; Byte langen Bereich an die Adresse, die durch <ziel> angegeben wird
    ;
    ; Im Fehlerfall liefert die Routine FALSE (Fehlercode über: xms_errormsg
    ; erreichbar), sonst TRUE.
    ;
    ; In:  quellhandle - das Handle, eines durch Fkt. 09h allokierten EMB
    ;      quelloffset - das Offset innerhalb des EMB
    ;      ziel        - Zeiger auf Zielbereich (im 1 MB Adressraum)
    ;      size        - die Länge des zu kopierenden Bereichs
    ; Out: TRUE        - Funktion erfolgreich
    ;      FALSE       - Fehler aufgetreten
    ;----------------------------------------------------------------
    xms_xms2ram proc quellhandle:word, quelloffset:dword,   \
                     ziel:dword,                            \
                     size:dword

                push ds si es di

    ; "Kopier-Struktur" mit Werten füllen
                mov ax,@data
                mov es,ax
                mov di,offset XMS_COPY

    ; Länge des zu kopierenden Bereiches
                mov ax,w [size+0]
                stosw
                mov ax,w [size+2]
                stosw

    ; Quell Handle setzen
                mov ax,quellhandle
                stosw

    ; Quell-"Offset" setzen
                mov ax,w [quelloffset+0]
                stosw
                mov ax,w [quelloffset+2]
                stosw

    ; Ziel-"Handle"
                mov ax,0
                stosw

    ; Ziel-"Offset"
                mov ax,w [ziel+0]
                stosw
                mov ax,w [ziel+2]
                stosw

    ; Funktion 0Bh aufrufen
                mov ax,@data
                mov ds,ax

                mov ah,0Bh
                mov si,offset XMS_COPY
                call [dword ptr XMS_ADR]

                cmp ax,0
                je xms2ram_quit_false

    ; Funktion fehlerfrei
                mov ax,TRUE
                jmp xms2ram_quit

    xms2ram_quit_false:
                mov XMS_ERROR,bl
                mov ax,FALSE

    xms2ram_quit:
                pop di es si ds
                ret
                endp

    ;----------------------------------------------------------------
    ; int xms_xms2xms (int quellhandle, long quelloffset,
    ;                  int zielhandle,  long zieloffset,
    ;                  long size);
    ;
    ; kopiert den durch <quellhandle> und <quelloffset>, <size>
    ; Byte langen Bereich in den durch <zielhandle> und <zieloffset>
    ; angegebenen EMB
    ;
    ; Im Fehlerfall liefert die Routine FALSE (Fehlercode über: xms_errormsg
    ; erreichbar), sonst TRUE.
    ;
    ; In:  quellhandle - das Quell Handle, eines durch Fkt. 09h allokierten EMB
    ;      quelloffset - das Quell Offset innerhalb des "Quell"-EMB
    ;      zielhandle  - das Ziel Handle, eines durch Fkt. 09h allokierten EMB
    ;      zieloffset  - das Ziel Offset innerhalb des "Ziel"-EMB
    ;      size        - die Länge des zu kopierenden Bereichs
    ; Out: TRUE        - Funktion erfolgreich
    ;      FALSE       - Fehler aufgetreten
    ;----------------------------------------------------------------
    xms_xms2xms proc quellhandle:word, quelloffset:dword,   \
                     zielhandle:word, zieloffset:dword,     \
                     size:dword

                push ds si es di

    ; "Kopier-Struktur" mit Werten füllen
                mov ax,@data
                mov es,ax
                mov di,offset XMS_COPY

    ; Länge des zu kopierenden Bereiches
                mov ax,w [size+0]
                stosw
                mov ax,w [size+2]
                stosw

    ; Quell Handle setzen
                mov ax,quellhandle
                stosw

    ; Quell-"Offset" setzen
                mov ax,w [quelloffset+0]
                stosw
                mov ax,w [quelloffset+2]
                stosw

    ; Ziel-"Handle"
                mov ax,zielhandle
                stosw

    ; Ziel-"Offset"
                mov ax,w [zieloffset+0]
                stosw
                mov ax,w [zieloffset+2]
                stosw

    ; Funktion 0Bh aufrufen
                mov ax,@data
                mov ds,ax

                mov ah,0Bh
                mov si,offset XMS_COPY
                call [dword ptr XMS_ADR]

                cmp ax,0
                je xms2xms_quit_false

    ; Funktion fehlerfrei
                mov ax,TRUE
                jmp xms2xms_quit

    xms2xms_quit_false:
                mov XMS_ERROR,bl
                mov ax,FALSE

    xms2xms_quit:
                pop di es si ds
                ret
                endp

    ;----------------------------------------------------------------
    ; int xms_lock (int handle, int far *adress);
    ;
    ; Speicherblock gegen verschieben sichern und Startadresse liefern
    ;
    ; In:       handle
    ;           (far) Zeiger auf DWORD !!! (wird gesetzt)
    ; Out:      Erfolgsmeldung / Fehlercode
    ;----------------------------------------------------------------
    xms_lock    proc handle:word, p:dword
                push ds es di
                mov ax,@data
                mov ds,ax

    ; Funktion 0Ch: XMS-Speicherblock gegen Verschieben sichern
                mov ah,0Ch
                mov dx,handle
                call [dword ptr XMS_ADR]

                cmp ax,0
                je lock_quit_false

    ; Funktion fehlerfrei
                mov ax,TRUE
                shl edx,16
                mov dx,bx                           ; edx = lineare Adresse

                les di,p
                mov dword ptr es:[di],edx
                jmp lock_quit

    lock_quit_false:
                mov XMS_ERROR,bl
                mov ax,FALSE

    lock_quit:
                pop di es ds
                ret
                endp

    ;----------------------------------------------------------------
    ; int xms_unlock (int handle);
    ;
    ; Speicherblock gegen verschieben sichern und Startadresse liefern
    ;
    ; In:       handle
    ; Out:      Erfolgsmeldung / Fehlercode
    ;----------------------------------------------------------------
    xms_unlock  proc handle:word
                push ds es di
                mov ax,@data
                mov ds,ax

    ; Funktion 0Dh: Gesperrten XMS-Speicherblock entsichern
                mov ah,0Dh
                mov dx,handle
                call [dword ptr XMS_ADR]

                cmp ax,0
                je unlock_quit_false

    ; Funktion fehlerfrei
                mov ax,TRUE
                jmp unlock_quit

    unlock_quit_false:
                mov XMS_ERROR,bl
                mov ax,FALSE

    unlock_quit:
                ret
                pop di es ds
                endp

    ;----------------------------------------------------------------
    ; int xms_errormsg (char far msg[80]);
    ;
    ; liefert den aktuelle Errorcode sowie einen Errorcode String
    ;
    ; In:       msg - (far) zeiger auf einen 80 Byte großen String
    ; Out:      letzter Errorcode
    ;----------------------------------------------------------------
    xms_errormsg proc msg:dword
                push ds es si di
                les di,msg

                mov si,@data
                mov ds,si

                mov bl,XMS_ERROR
                and bx,0FFh
                sub bx,080h

                cmp bx,32h
                jbe nummer_ok

    ; Offset für Fehlermeldung: "ungültiger Fehlercode"                                        
                mov bx,32h          

    nummer_ok:
                push bx
                shl bx,1
                mov si,[XMS_MSGTAB+bx]

                mov cx,40
                rep movsw

                pop ax
                add ax,80h
                pop di si es ds
                ret
                endp

                end


