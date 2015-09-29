;----------------------------------------------------------------------
; EMS.ASM - Demonstration der Nutzung eines Expanded Memory Managers (EMM)
;
; Die hier definierten Assembleroutinen sollten mit einem C-Compiler
; (mit FAR-Speichermodell, z.B. LARGE) verwendet werden.
; Bei der Verwendung anderer Speichermodelle oder Compiler müssen
; wahrscheinlich Änderungen vorgenommen werden.
;
; Führt eine der Funktionen zu einem EMM-Fehlercode, so liefert die entspr.
; Funktion den Wert FALSE und speichert den Fehlercode in EMM_ERROR.
; Ein Fehler kann bei Bedarf über die Funktion ems_errormsg genauer
; identifiziert werden.
;
; ASSEMBLIEREN: TASM /ML EMS.ASM
;
; (c) Copyright 1996 by Jens Hohmuth. All Rights Reserved.
; email: Jens.Hohmuth@fh-zwickau.de
;
; Erste Version:     04.10.96
; Letze Aenderungen: 07.10.96
;----------------------------------------------------------------------
                .model large, c
                .stack 100h
    TRUE        equ 1
    FALSE       equ 0

    public      ems_check,          \
                ems_version,        \
                ems_allpages,       \
                ems_freepages,      \
                ems_alloc,          \
                ems_free,           \
                ems_setmap,         \
                ems_pageframe,      \
                ems_errormsg

                .data
    ; Erkennungsstring
    EMM_STRING  db "EMMXXXX0"

    ; enthält aktuelle (letzte) aufgetretene EMM-Fehlernummer
    EMM_ERROR   db 0

    ; Offsettabelle für ems_errormsg Funktion
    EMM_MSGTAB  dw error80, error81, error82, error83, error84, error85
                dw error86, error87, error88, error89, error8A, error8B
                dw error8C, error8D, error8E, error8F
                dw error_nomsg
    error80     db "Softwareschnittstelle fehlerhaft oder interner Fehler !                        ", 0
    error81     db "EMS-Hardware fehlerhaft !                                                      ", 0
    error82     db "EMM ist beschaeftigt !                                                         ", 0
    error83     db "Handle ist ungueltig !                                                         ", 0
    error84     db "Funktionsnummer ist  ungueltig bzw. wird nicht unterstuetzt !                  ", 0
    error85     db "keine weiteren Handles mehr verfuegbar !                                       ", 0
    error86     db "Fehler beim Sichern oder Zuruecksetzen der Abb. zwischen log. und phys. Seiten!", 0
    error87     db "zu wenig freie Seiten verfuegbar !                                             ", 0
    error88     db "Ungueltige Seitenanzahl !                                                      ", 0
    error89     db "es wurde versucht, 0 Seiten zu belegen !                                       ", 0
    error8A     db "Seitenummer ungueltig !                                                        ", 0
    error8B     db "Physikalische Seitenummer ungueltig !                                          ", 0
    error8C     db "Mapping kann nicht gesichert werden !                                          ", 0
    error8D     db "Mapping ist bereits gesichert !                                                ", 0
    error8E     db "Mapping war nicht gesichert !                                                  ", 0
    error8F     db "Falsche Unterfunktions-Nummer !                                                ", 0
    error_nomsg db "Ungueltiger Fehlercode ?                                                       ", 0

                .code
    ;------------------------------------------------------------------
    ; int ems_check (void);
    ;
    ; versucht über die EMM-Treiber Kennung "EMMXXXX0" einen EMM zu
    ; identifizieren
    ;
    ; In:       void
    ; Out:      TRUE  (=1), wenn erfolgreich
    ;           FALSE (=0), wenn kein EMM gefunden werden konnte
    ;------------------------------------------------------------------
    ems_check   proc
                push ds si es di

    ; Segmentadresse der Interruptroutine (Int 67h) ermitteln
                mov ah,35h
                mov al,67h
                int 21h

    ; es= Segmentadresse
    ; bx= Offsetadresse der Routine (wird nicht benötigt)
	
    ; testen ob es:000Ah = Kennung 'EMMXXXX0' enthält
                push es
                pop ds
                mov si,0Ah

                mov di,seg EMM_STRING
                mov es,di
                mov di,offset EMM_STRING

                mov cx,8
                repe cmpsb
                jcxz gefunden

    nicht_gefunden:
                mov ax,FALSE
                jmp quit

    gefunden:
                mov ax,TRUE

    quit:
                pop di es si ds
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_version (int far *version);
    ;
    ; benutzt EMM-Fkt. 46h, um Treiber Version zu ermitteln
    ;
    ; In:       version - (far) Zeiger auf Word Speicherplatz
    ; Out:      TRUE    - Fkt. war erfolgreich:
    ;                     (<Version> enthält Treiberversion)
    ;           FALSE   - Fkt. war nicht erfolgreich
    ;------------------------------------------------------------------
    ems_version proc ver:dword
                push es ds di

    ; es:di = Zeiger auf version
                les di,ver

    ; Versionsummer ermitteln
                mov ah,46h
                int 67h

    ; EMM-Error aufgetreten (ah <> 0) ?
                cmp ah,0
                je version_ok

    ; Fehlercode unter EMM_ERROR speichern und Funktion
    ; mit FALSE verlassen
                mov bx,@data
                mov ds,bx
                mov EMM_ERROR,ah

                mov ax,FALSE
                jmp version_quit
	
    version_ok:

    ; gepackten BCD-Code in al in ungepackten BCD-Code in ax
    ; umwandeln und ...
                and ax,0FFh
                shl ax,4
                shr al,4

    ; unter <ver> speichern
                stosb
                mov al,ah
                stosb

    ; Funktion erfolgreich, TRUE liefern
                mov ax,TRUE

    version_quit:
                pop di ds es
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_pageframe (int *far pageframe);
    ;
    ; benutzt EMM-Fkt. 41h, um aktuelle PageFrame Adresse bereitzustellen
    ;
    ; In:       pageframe - (far) Zeiger auf Word Speicherplatz
    ; Out:      TRUE      - Fkt. war erfolgreich:
    ;                       (Pageframe die Segmentadresse des "Page-Frames")
    ;           FALSE     - Fkt. war nicht erfolgreich
    ;------------------------------------------------------------------
    ems_pageframe proc pageframe:dword
                push es ds di

    ; es:di = Zeiger auf pageframe
                les di,pageframe

    ; pageframe ermitteln
                mov ah,41h
                int 67h

    ; EMM-Error aufgetreten (ah <> 0) ?
                cmp ah,0
                je pageframe_ok

    ; Fehlercode unter EMM_ERROR speichern und Funktion
    ; mit FALSE verlassen
                mov bx,@data
                mov ds,bx
                mov EMM_ERROR,ah

                mov ax,FALSE
                jmp pageframe_quit
	
    pageframe_ok:

    ; unter <pageframe> speichern
                mov ax,bx
                stosw

    ; Funktion erfolgreich, TRUE liefern
                mov ax,TRUE

    pageframe_quit:
                pop di ds es
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_freepages (int far *freepages);
    ; 
    ; ermittelt die Anzahl EMS-Seiten die noch zur Verfügung stehen
    ;
    ; In:       freepages - (far) zeiger auf Int-Speicherplatz
    ; Out:      TRUE      - Funktion erfolgreich
    ;           FALSE     - Fehler
    ;------------------------------------------------------------------
    ems_freepages proc freepages:dword
                push es ds di
                les di,freepages

                mov ah,42h
                int 67h

                cmp ah,0
                je freepages_ok

    ; Fehlercode unter EMM_ERROR speichern und Funktion
    ; mit FALSE verlassen
                mov bx,@data
                mov ds,bx
                mov EMM_ERROR,ah

                mov ax,FALSE
                jmp freepages_quit

    freepages_ok:
                mov ax,bx
                stosw

                mov ax,TRUE

    freepages_quit:
                pop di ds es
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_allpages (int far *allpages);
    ; 
    ; ermittelt die Gesamtanzahl der EMS-Seiten
    ;
    ; In:       allpages - (far) zeiger auf Int-Speicherplatz
    ; Out:      TRUE     - Funktion erfolgreich
    ;           FALSE    - Fehler
    ;------------------------------------------------------------------
    ems_allpages proc allpages:dword
                push es ds di
                les di,allpages

                mov ah,42h
                int 67h

                cmp ah,0
                je allpages_ok

    ; Fehlercode unter EMM_ERROR speichern und Funktion
    ; mit FALSE verlassen
                mov bx,@data
                mov ds,bx
                mov EMM_ERROR,ah

                mov ax,FALSE
                jmp allpages_quit

    allpages_ok:
                mov ax,dx
                stosw

                mov ax,TRUE

    allpages_quit:
                pop di ds es
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_alloc (int far *handle, int ssize);
    ; 
    ; allokiert die Anzahl angegebener Seiten im EMS und liefert Handle
    ;
    ; In:       handle - (far) Zeiger auf Int-Speicherplatz (Handle Adresse)
    ;           ssize  - Anzahl zu allokierender Seiten
    ; Out:      TRUE   - Funktion erfolgreich
    ;           FALSE  - Fehler
    ;------------------------------------------------------------------
    ems_alloc   proc handle:dword, ssize:word
                push es ds di
                les di,handle

                mov ah,43h
                mov bx,ssize
                int 67h

                cmp ah,0
                je alloc_ok

    ; Fehlercode unter EMM_ERROR speichern und Funktion
    ; mit FALSE verlassen
                mov bx,@data
                mov ds,bx
                mov EMM_ERROR,ah

                mov ax,FALSE
                jmp alloc_quit

    alloc_ok:
                mov ax,dx
                stosw

                mov ax,TRUE

    alloc_quit:
                pop di ds es
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_free (int handle);
    ; 
    ; freigeben des durch das uebergebene Handle allokierten Speicher-
    ; bereiches
    ;
    ; In:       handle - Handle
    ; Out:      TRUE   - Funktion erfolgreich
    ;           FALSE  - Fehler
    ;------------------------------------------------------------------
    ems_free    proc handle:word
                push ds
                mov ah,45h
                mov dx,handle
                int 67h

                cmp ah,0
                je free_ok

    ; Fehlercode unter EMM_ERROR speichern und Funktion
    ; mit FALSE verlassen
                mov bx,@data
                mov ds,bx
                mov EMM_ERROR,ah

                mov ax,FALSE
                jmp free_quit

    free_ok:
                mov ax,TRUE

    free_quit:
                pop ds
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_setmap (int handle, int phys, int log);
    ; 
    ; abbilden einer logischen Page auf eine physische
    ;
    ; In:       handle - Handle
    ;           phys   - physische Seite (0 bis 3)
    ;           log    - logische Seite (im EMS)
    ; Out:      TRUE   - Funktion erfolgreich
    ;           FALSE  - Fehler
    ;------------------------------------------------------------------
    ems_setmap  proc handle:word, phys:word, log:word
                push ds
                mov ax,phys
                mov dx,handle
                mov bx,log
                mov ah,44h
                int 67h

                cmp ah,0
                je setmap_ok

    ; Fehlercode unter EMM_ERROR speichern und Funktion
    ; mit FALSE verlassen
                mov bx,@data
                mov ds,bx
                mov EMM_ERROR,ah

                mov ax,FALSE
                jmp setmap_quit

    setmap_ok:
                mov ax,TRUE

    setmap_quit:
                pop ds
                ret
                endp

    ;------------------------------------------------------------------
    ; int ems_errormsg (char far msg[80]);
    ;
    ; liefert den aktuelle Errorcode sowie einen Errorcode String
    ;
    ; In:       msg - (far) Zeiger auf einen 80 Byte großen String
    ; Out:      letzter Errorcode
    ;------------------------------------------------------------------
    ems_errormsg proc msg:dword
                push ds es si di
                les di,msg

                mov si,@data
                mov ds,si

                mov bl,EMM_ERROR
                and bx,0FFh
                sub bx,080h
                cmp bx,0fh
                jbe nummer_ok

    ; Offset für die Fehlermeldung: "ungültiger Fehlercode"
                mov bx,10h          

    nummer_ok:
                push bx
                shl bx,1
                mov si,[EMM_MSGTAB+bx]

                mov cx,40
                rep movsw

                pop ax
                add ax,80h
                pop di si es ds
                ret
                endp

                end


