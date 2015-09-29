/*
** PM_01.C - Einschalten des Protected Mode unter C
**           (Nutzt soviel wie moeglich C (BC 3.1 !) )
**
** (ACHTUNG: - MUSS IM SMALL-MEMORY MODELL COMPILIERT WERDEN !!!
**           - SOLLTE KEINE RUN-TIME STACK-öBERPRöFUNG BENUTZEN !!!
**           - MUSS 80386-CODE COMPILIEREN KOENNEN !!!
**           - MUSS MIT EXTERNEM (TASM) ASSEMBLIERT WERDEN,
**             Options | Compiler | Code generation | Compile via Assembler !!!
**           - EMM386 (oder Ñhnliche Programme) dÅrfen nicht aktiv sein !!!)
**
** Erste Version:     16.08.97
** Letze énderungen:  17.08.97 - Programm verwendet jetzt Pseudo-Register-
**                               variablen und nicht mehr segread
**                    20.08.97 - Makro "color" um die Bildschirmfarbe zu
**                               verÑndern
**                             - Makro "waitkey" zum Warten auf Tastendruck
**                             ==> Festellen wie weit das Programm im
**                                 (nicht debugbaren) Prot. Mode funktioniert
**                    22.08.97 - Funktion "print" zur direkten Stringausgabe
**                               (Programm schaltet jetzt in den pmode,
**                               gibt einen String aus und kehrt in den
**                               Realmode zurÅck)
**
** (c) Copyright 1997 by Jens Hohmuth. All rights reserved.
** email: Jens.Hohmuth@fh-zwickau.de
*/

#include <stdio.h>
#include <conio.h>                  /* fÅr Portbefehle inp / outp        */
#include <dos.h>                    /* fÅr Interrupts: disable / enable  */
#include <string.h>                 /* fÅr strlen                        */

/* Constants */
#define MAX_DESKRIPTORS     6

#define sNIL                0x00
#define sCODE               0x08
#define sVIDEO              0x10
#define sDATA               0x18
#define sSTACK				0x20
#define sCODE_ALIAS         0x28

/* Deskriptortyp Byte (Bit Codierung); Erlaubt Konstruktionen wie:	*/
/* segmenttyp= DATASEG+ PRESENT+ DPL0+ SEGMENT 		                */
#define ACCESSED			0x01
#define DATASEG_NOTREADABLE	0x00
#define DATASEG				0x02
#define DATASEG_RESERVED	0x04
#define DATASEG_EXPANDABLE	0x06				/* "expand-down" Datensegment */
#define CODESEG_NOTREADABLE	0x08
#define CODESEG				0x0A
#define CODESEG_CONFORM_NR	0x0C
#define CODESEG_CONFORM		0x0E
#define SEGMENT				0x10
#define SYSTEM				0x00
#define DPL0				0x00
#define DPL1				0x20
#define DPL2				0x40
#define DPL3				0x60
#define PRESENT				0x80

/* Spezial-Info (Bit Codierung); Erlaubt Konstruktionen wie:    */
/* segmentspezialinfo= GRAN+ _80386                             */
#define GRAN                0x40
#define _80386              0x30
#define AVL                 0x10

/* Datentyp Definitonen */
typedef unsigned char		BYTE;
typedef unsigned short int	WORD;
typedef unsigned long int	DWORD;

/* Strukturen fuer GDT-Eintrage ... */
typedef struct
	{
	WORD	segsize_0_15;						/* Segment-Size Bit  0..15 */
	WORD	segbase_0_15;						/* Segment-Base Bit  0..15 */
	BYTE	segbase_16_23;						/* Segment-Base Bit 16..23 */
	BYTE	segtype;							/* Segment-Typ             */
	BYTE	segsize_16_19;						/* Segment-Size Bit 16..19 */
												/* Special Info. G-Bit etc.*/
	BYTE	segbase_24_31;						/* Segment-Base Bit 24..31 */
	} SEG_DESKRIPTOR;

typedef struct
	{
	WORD	limit;
	DWORD	base;
	} GDTSTRUC;

SEG_DESKRIPTOR  gdt [MAX_DESKRIPTORS];
WORD            rmode_cs;
WORD            rmode_ds;
WORD            rmode_es;
WORD            rmode_ss;

/* éndert DAC-Color 0 (Hintergrund) auf neuen (ROT-) Wert c */
#define color(c)                    \
        outp (0x03c8, 0);           \
        outp (0x03c9, c);           \
        outp (0x03c9, 0);           \
        outp (0x03c9, 0);

/*
** Warten auf ESC-Taste im Protected Mode; da weder DOS noch BIOS im
** pmode funktionieren wird hier direkt der Tastaturcontroller
** abgefragt und solange gewartet bis ESC gedrÅckt und wieder los-
** gelassen wird
*/
#define waitkey                     \
    while (inp (0x60) != 1);        \
    while (inp (0x60) != 0x80 + 1);

/*
** setzen von Deskriptor-Attributen
**
** In:  des     - Zeiger auf Deskriptor-Strukture im Speicher (in die GDT)
**      size    - Groesse des Segmentes in Byte (0..1MB)
**      segment - (Realmode) Segmentadresse des Segment-Speichers
**      offset  - (Realmode) Offsetadresse des Segment-Speichers
**      type    - Segment-Type Byte
**      gran    - Zusaetzliches Info. Byte (Granularity, 80386 Segment ...)
**                (High-Nibble des Byte 7)
** Out: nil
*/
void deskr_setattrib (SEG_DESKRIPTOR *des, DWORD size, DWORD segment, DWORD offset, BYTE type, BYTE gran)
	{
	DWORD seg_base;

	/* Setzen der Segment-Groesse */
	des->segsize_0_15=  size & 0xFFFF;
	des->segsize_16_19= (size >> 16) & 0xFF;

	/* Setzen des Segmenttypes */
	des->segtype= type;

	/* Setzen des Spezial-Informations Nibble */
	des->segsize_16_19+= gran;

	/* Berechnen und Setzen der Segment-Basisadresse */
	seg_base= (segment << 4) + offset;
	des->segbase_0_15= seg_base & 0xFFFF;
	des->segbase_16_23= (seg_base >> 16) & 0xFF;
	des->segbase_24_31= (seg_base >> 24) & 0xFF;
	}

/*
** Adresse der GDT Berechnen und mit LGDT setzen
*/
void lgdt (void)
	{
	GDTSTRUC	gdt_adr;
	DWORD		segment, offset;

    gdt_adr.limit= MAX_DESKRIPTORS*8;

    /* Auf die folgende Weise wird sichergestellt,      */
    /* dass zur Adressrechnung 32Bit verwendet werden ! */
    segment= FP_SEG (&gdt[0]);
    offset = FP_OFF (&gdt[0]);
	gdt_adr.base = ( segment << 4) + offset;

    /* GDT Adresse setzen; mit dem (Assembler-) Befehl LGDT */
    /*   (Hier geht's wirklich nicht ohne Assembler ;-) )   */
    asm {
		lgdt gdt_adr
		}
	}

/*
** Protected Mode Einschalten:
**
**      - Interrupts Ausschalten (bleiben waehrend der gesamten Zeit im
**        Protected Mode ausgeschaltet)
**      - Bit 1 im Register CR0 setzen
**      - FAR-JMP Ausfuehren zu pmode-Codesegment Selektor: sCODE !
**      - Selektor (bzw. Segment-) Register auf neue Werte setzen mit Hilfe
**        der Pseudo-Register Variablen
*/
void enable_pmode (void)
     {
     /* aktuelle Realmode Segmentadressen sichern */
     rmode_cs= _CS;
     rmode_ds= _DS;
     rmode_es= _ES;
     rmode_ss= _SS;

     /* Interrupts aus */
     disable ();

     /* pmode init: CR0, Bit 1 setzen */
     asm {
         mov eax,cr0
         or eax,1
         mov cr0,eax
         }

     /*
     ** Far-Jump um Warteschlange zu leeren und Codesegment-Selektor
     ** zu setzen;
     ** (Im Fall, dass die Codesegment-Attribute des Realmode-Segmentes
     ** beibehalten werden koennten, also die Eigenschaften des PMode-
     ** Segmentes (z.B. Groesse) dem Realmode-Segment entsprechen, koennten
     ** die folgenden Anweisungen durch ein einfaches GOTO ersetzt werden ! )
     */
     asm {
         db 0x0ea                               /* assembliert FAR-JMP */
         dw offset pmode                        /* zur Adresse:        */
         dw sCODE                               /*        sCODE:Offset */

     pmode:
         }

     /* Selektor-Register (Segmentadressen) mit gueltigen PMode-Werten */
     _DS= sDATA;
     _ES= sDATA;
     _SS= sSTACK;
     }

/*
** Protected Mode Ausschalten:
**
**      - Bit 1 im Register CR0 loeschen
**      - FAR-JMP Ausfuehren, zurueck in den Realmode
**      - Interrupts wieder an
*/
void disable_pmode (void)
    {
    WORD save_cs;
    WORD save_ds;

    /*
    ** Ruecksprung vorbereiten
    ** (Realmode-) Codesegment-Wert unter rm_cs (in dieser Fkt. !)
    ** wiederherstellen
    */
    save_cs= rmode_cs;                          /* Werte lokal sichern      */
    save_ds= rmode_ds;

    /*
    ** Datensegment auf dieses Codesegment zeigen lassen
    ** (Im Protected Mode darf in ein Codesegment nicht geschrieben werden)
    ** Hier wird deshalb ein Alias Datensegment (das auf das eigentliche
    ** Codesegment zeigt) benutzt.
    */
    _DS= sCODE_ALIAS;
    asm mov ax,save_cs
    asm mov di,offset cs:rm_cs
    asm mov [di],ax                             /* altes Codesegment herstellen */

    asm mov ax,save_ds
    asm mov di,offset cs:rmode+ 1
    asm mov [di],ax                             /* altes Datensegment herstellen */

    /* Standard (64 KB) Datensegment (mîglicherweise verÑnderte) Selektorregister */
    _DS= sDATA;
    _ES= sDATA;

    /* CR0 Bit 1 loeschen */
    asm {
        mov eax,cr0
        and eax,NOT 1
        mov cr0,eax
        }

    /* "FAR-JMP" in den Realmode */
    asm             db 0x0ea
    asm             dw offset rmode
    asm rm_cs       dw 0                        /* wird modifiziert ... */
    asm rmode:
    asm             mov ax,0                    /* wird modifiziert ... */
    asm             mov ds,ax

    /* Segmentregister wieder mit Realmode-Segmentadressen initialisieren */
    _ES= rmode_es;
    _SS= rmode_ss;

    /* Interrupts wieder an */
    enable ();
    }

/*
** print - Stringausgabe Åber Videosegment
**
** In:  x - Ausgabeposition X  
**      y - Ausgabeposition Y
**      s - String
**      a - Attribut (-1 fÅr keine énderung)
** Out: nil
*/
void print (int x, int y, char *s, int a)
    {
    char far *video;
    int i;

    /* Erzeugt einen FAR-Zeiger auf das Videosegment            */
    /* (Man beachte, dass als Segmentadresse der Selektorwert   */
    /* des Videosegmentes 0x10 eingetragen wird !)              */
    video= (char far *) MK_FP (sVIDEO, y*160+ x*2);

    if (a == -1)
        {
        /* keine énderung der Attributwerte */
        for (i=0; i<strlen(s); i++)
            {
            *(video+ 0)= s[i];
            video+= 2;
            }
        }
    else
        {
        /* Mit énderung der Attributwerte */
        for (i=0; i<strlen(s); i++)
            {
            *(video+ 0)= s[i];
            *(video+ 1)= (char)a;
            video+= 2;
            }
        }
    }

void main (void)
	{
	/*
    ** Deskriptoren anlegen und mit Werten fuellen
    **
    ** ( >> 3 ) wird benoetigt um den korrekten Index 0,1,2 ... zu
    ** berechnen (Die Konstanten, z.B. sDATA, stellen den Selektor-
    ** wert da. Dabei werden nur die Bit 2..15 als Index verwendet.
    ** Bit 0..2 halten andere Funktionen fest. Die Verschiebung um
    ** 3 ist notwendig um den Index in das GDT-Array zu erhalten.
    */
	deskr_setattrib (&gdt[sNIL>>3],  0, 0, 0, 0, 0);
    deskr_setattrib (&gdt[sCODE>>3], 0xFFFF,  _CS, 0, CODESEG+ SEGMENT+ DPL0+ PRESENT, 0);
    deskr_setattrib (&gdt[sDATA>>3], 0xFFFF,  _DS, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);
    deskr_setattrib (&gdt[sSTACK>>3], 0xFFFF, _SS, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);
	deskr_setattrib (&gdt[sVIDEO>>3], 4000, 0xB800, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);
    deskr_setattrib (&gdt[sCODE_ALIAS>>3], 0xFFFF, _CS, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);

	/* GDT Adresse setzen */
	lgdt ();

    /* init pmode */
    enable_pmode ();

    /* Stringausgabe */
    print (35, 24, "Hallo Welt vom Protected Mode unter BC 3.1 !", -1);

    /* deinit pmode */
    disable_pmode ();
	}
