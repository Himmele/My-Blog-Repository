/*
** PM_02.C - Einschalten des Protected Mode unter C und Einrichten einer IDT
**         - Programm wartet anschlie·end (im pmode!) in einer Warteschleife
**         - Timer-Interrupt 8 (automatisch 18.2 mal pro Sekunde aktiviert)
**           wird ausgelîst und fÅhrt zu einer Exception 8 !
**
**           (Das Programm stellt trotzdem fÅr alle anderen Exceptions
**            entsprechende Handler bereit, es ist deshalb fÅr weitere
**            Experimente geeignet !)
**
** ACHTUNG - Darf keinen Standard Stack Rahmen verwenden !
**
** Erste Version:     23.08.97
** Letze énderungen:  25.08.97
**
** (c) Copyright 1997 by Jens Hohmuth. All rights reserved.
** email: Jens.Hohmuth@fh-zwickau.de
*/

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>                            /* fÅr Interrupts: disable / enable  */
#include <string.h>                         /* fÅr strlen (fÅr Funktion print)   */

/* Konstante */
#define MAX_DESKRIPTORS     6
#define MAX_IDTGATES        17

#define sNIL                0x00            /* Selektorwerte */
#define sCODE               0x08
#define sVIDEO              0x10
#define sDATA               0x18
#define sSTACK				0x20
#define sCODE_ALIAS         0x28

/* Deskriptortyp-Byte (Bit Codierung); Erlaubt Konstruktionen wie:  */
/* segmenttyp= DATASEG+ PRESENT+ DPL0+ SEGMENT 		                */
#define ACCESSED			0x01
#define DATASEG_NOTREADABLE	0x00
#define DATASEG				0x02
#define DATASEG_RESERVED	0x04
#define DATASEG_EXPANDABLE  0x06            /* "expand-down" Datensegment */
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

/* Gate (-Deskriptor) Typen ... */
#define TSS_80286			0x01
#define LDT					0x02
#define TSS_80286_ACTIVE	0x03
#define CALLGATE_80286		0x04
#define TASKGATE			0x05
#define INTGATE_80286		0x06
#define TRAPGATE_80286		0x07
#define TSS_80386			0x09
#define TSS_80386_ACTIVE	0x0B
#define CALLGATE_80386		0x0C
#define INTGATE_80386		0x0E
#define TRAPGATE_80386		0x0F

/* Spezial-Info (Bit Codierung); Erlaubt Konstruktionen wie:    */
/* segmentspezialinfo= GRAN+ _80386                             */
#define GRAN				0x40;
#define _80386				0x30;
#define AVL					0x10;

/* Datentyp Definitonen */
typedef unsigned char		BYTE;
typedef unsigned short int	WORD;
typedef unsigned long int	DWORD;

/* Strukturen fuer GDT-Eintrage ... */
typedef struct
	{
    WORD    segsize_0_15;                   /* Segment-Size Bit  0..15 */
    WORD    segbase_0_15;                   /* Segment-Base Bit  0..15 */
    BYTE    segbase_16_23;                  /* Segment-Base Bit 16..23 */
    BYTE    segtype;                        /* Segment-Typ             */
    BYTE    segsize_16_19;                  /* Segment-Size Bit 16..19 */
                                            /* Spezial Info. G-Bit etc.*/
    BYTE    segbase_24_31;                  /* Segment-Base Bit 24..31 */
	} SEG_DESKRIPTOR;

/* Struktur zum Setzen von GDT und IDT */
typedef struct
	{
	WORD	limit;
	DWORD	base;
	} XDTSTRUC;

/* Strukturen fÅr IDT */
typedef struct
	{
    WORD    offset_0_15;
    WORD	selektor;
    BYTE	reserved;
    BYTE	type;
    WORD	offset_16_31;
    } GATE_DESKRIPTOR;

 SEG_DESKRIPTOR gdt [MAX_DESKRIPTORS];
GATE_DESKRIPTOR	idt [MAX_IDTGATES];
WORD            rmode_cs;
WORD            rmode_ds;
WORD            rmode_es;
WORD            rmode_ss;

char			exception_txt[][40]=
				{ {"00: Divisionsfehler                   \0"},
				  {"01: Debugger-Interrupt                \0"},
				  {"02: NMI                               \0"},
                  {"03: Breakpoint                        \0"},
                  {"04: öberlauf (INTO)                   \0"},
                  {"05: Arraygrenzen Åberschritten (BOUND)\0"},
                  {"06: ungÅltiger Opcode                 \0"},
                  {"07: Coprozessor nicht installiert     \0"},
                  {"08: Doppelfehler                      \0"},
                  {"09: Segment-öberlauf des Coprozessor  \0"},
                  {"10: TSS ungÅltig                      \0"},
                  {"11: Segment nicht im Hauptspeicher    \0"},
                  {"12: Stack-öberlauf                    \0"},
                  {"13: allgemeine Schutzverletzung       \0"},
                  {"14: Paging Ausnahmezustand            \0"},
                  {"15: reserviert                        \0"},
                  {"16: Coprozessor-Fehler                \0"},
                  {"17: Ausrichtungfehler                 \0"} };

/*
** Setzen von Deskriptor-Attributen
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
void deskr_setattr (SEG_DESKRIPTOR *des, DWORD size, DWORD segment, DWORD offset, BYTE type, BYTE gran)
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
** Setzen von Gate-Deskriptor Attributen
**
** In:  des     - Zeiger auf Deskriptor-Strukture im Speicher (in die IDT)
**      selektor- (Code-) Segmentselektor
**      off     - Offset in das entsprechende Segment (Funktionszeiger)
**      type    - Gatedeskriptortype Byte
**
** Out: nil
*/
void gate_setattr (GATE_DESKRIPTOR *des, WORD selektor, void (*off) (void), BYTE type)
	{
	DWORD offset= FP_OFF (off);

    /* Setzen des Selektors */
	des->selektor= selektor;

    /* Setzen des entsprechenden Segment-Offsets */
	des->offset_0_15=  offset & 0xFFFF;
	des->offset_16_31= (offset >> 16) & 0xFFFF;

	/* Setzen des Segmenttypes */
	des->type= type;
	}

/*
** Adresse der GDT Berechnen und mit LGDT setzen
*/
void lgdt (void)
	{
	XDTSTRUC	gdt_adr;
	DWORD		segment, offset;

    gdt_adr.limit= MAX_DESKRIPTORS*8;

    /* Auf die folgende Weise wird sichergestellt,      */
    /* dass zur Adressrechnung 32Bit verwendet werden ! */
    segment= FP_SEG (&gdt[0]);
    offset = FP_OFF (&gdt[0]);
	gdt_adr.base = ( segment << 4) + offset;

    /* GDT Adresse setzen; mit dem (Assembler-) Befehl LGDT */
    /*   (Hier geht es wirklich nicht ohne Assembler ;-) )   */
    asm lgdt gdt_adr
	}

/*
** Adresse der IDT Berechnen und mit LIDT setzen
*/
void lidt (void)
	{
	XDTSTRUC	idt_adr;
	DWORD		segment, offset;

    idt_adr.limit= MAX_IDTGATES*8;

    /* Auf die folgende Weise wird sichergestellt,      */
    /* dass zur Adressrechnung 32Bit verwendet werden ! */
    segment= FP_SEG (&idt[0]);
    offset = FP_OFF (&idt[0]);
	idt_adr.base = ( segment << 4) + offset;

    /* GDT Adresse setzen; mit dem (Assembler-) Befehl LGDT */
    /*   (Hier geht es wirklich nicht ohne Assembler ;-) )   */
    asm lidt idt_adr
	}

/*
** Protected Mode Einschalten:
**
**      - Bit 1 im Register CR0 setzen
**      - FAR-JMP zu pmode-Codesegment Selektor: sCODE ausfuehren !
**      - Selektor (bzw. Segment-) Register auf neue Werte setzen mit Hilfe
**        der Pseudo-Register Variablen
*/
void enable_pmode (void)
     {
     disable ();

     /* aktuelle Realmode Segmentadressen sichern */
     rmode_cs= _CS;
     rmode_ds= _DS;
     rmode_es= _ES;
     rmode_ss= _SS;

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
	XDTSTRUC	idt_adr;
    WORD 		save_cs;
    WORD 		save_ds;

    disable ();

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

    /* Standard (64 KB) Datensegment (mîglicherweise verÑnderte) Selektorregister ! */
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

    /* IDT wieder auf Realmode Standard */
    idt_adr.limit= 0x03ff;
    idt_adr.base= 0;
	asm lidt idt_adr

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

void exception_all (int n)
	{
    /* Meldung ausgeben */
    print (0, 2, exception_txt[n], 0xC);

    /*
	** FÅr Hardware Interrupts, die eventuell eine Exception auslîsen kînnen,
    ** Interrupt bestÑtigen damit neue generiert werden kînnen.
    **
    ** Z.B. lîst dieses Programm den Interrupt 8 (Exception 8 - eigentlich
	** Timer-Interrupt) aus, da dieser Interrupt normalerweise 18.2 mal in
	** der Sekunde aktiv wird.
	** Durch Umprogrammieren der Interrupt-Controller ist es zwar mîglich
	** die Hardware-Interrupts entsprechend umzuleiten, doch sollte gerade
    ** die Auslîsung der Exception 8 in diesem Beispielprogramm demonstriert
	** werden.
    */
    outp (0x20, 0x20);
    outp (0xA0, 0x20);

    /* deinit pmode */
    disable_pmode ();

    /* Quit ... */
    exit (1);
    }

/*
** Exception Handler. Es wird zum allgemeinen Exceptionhandler verzweigt.
*/
void exception_00 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (0);
    }

void exception_01 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (1);
    }

void exception_02 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (2);
    }

void exception_03 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (3);
    }

void exception_04 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (4);
    }

void exception_05 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (5);
    }

void exception_06 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (6);
    }

void exception_07 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (7);
    }

void exception_08 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (8);
    }

void exception_09 (void)
	{
    /* disable Interrupts */
	disable ();

    /* Spez. Interrupt 9 (Tastatur) ==> Byte vom Tastaturcontroller lesen */
    inp (0x60);

    _DS= sDATA;
    exception_all (9);
    }

void exception_10 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (10);
    }

void exception_11 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (11);
    }

void exception_12 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (12);
    }

void exception_13 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (13);
    }

void exception_14 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (14);
    }

void exception_15 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (15);
    }

void exception_16 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (16);
    }

void exception_17 (void)
	{
    /* disable Interrupts */
	disable ();

    _DS= sDATA;
    exception_all (17);
    }

void main (void)
	{
	unsigned int i, j;

	/*
    ** Deskriptoren anlegen und mit Werten fuellen
    **
    ** ( >> 3 ) wird benoetigt um den korrekten Index 0,1,2 ... zu
    ** berechnen (Die Konstanten, z.B. sDATA, stellen den Selektor-
    ** wert dar. Dabei werden nur die Bit 2..15 als Index verwendet.
    ** Bit 0..2 halten andere Funktionen fest. Die Verschiebung um
    ** 3 Bit ist notwendig, um den Index in das GDT-Array zu erhalten.
    */
	deskr_setattr (&gdt[sNIL>>3],  0, 0, 0, 0, 0);
    deskr_setattr (&gdt[sCODE>>3], 0xFFFF,  _CS, 0, CODESEG+ SEGMENT+ DPL0+ PRESENT, 0);
    deskr_setattr (&gdt[sDATA>>3], 0xFFFF,  _DS, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);
    deskr_setattr (&gdt[sSTACK>>3], 0xFFFF, _SS, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);
	deskr_setattr (&gdt[sVIDEO>>3], 4000, 0xB800, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);
    deskr_setattr (&gdt[sCODE_ALIAS>>3], 0xFFFF, _CS, 0, DATASEG+ SEGMENT+ DPL0+ PRESENT, 0);

    /*
    ** IDT-(Interrupt-Gate-) Deskriptoren anlegen
	*/
    gate_setattr (&idt[ 0], sCODE, &exception_00, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 1], sCODE, &exception_01, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 2], sCODE, &exception_02, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 3], sCODE, &exception_03, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 4], sCODE, &exception_04, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 5], sCODE, &exception_05, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 6], sCODE, &exception_06, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 7], sCODE, &exception_07, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 8], sCODE, &exception_08, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[ 9], sCODE, &exception_09, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[10], sCODE, &exception_10, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[11], sCODE, &exception_11, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[12], sCODE, &exception_12, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[13], sCODE, &exception_13, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[14], sCODE, &exception_14, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[15], sCODE, &exception_15, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[16], sCODE, &exception_16, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);
    gate_setattr (&idt[17], sCODE, &exception_17, INTGATE_80386+ SYSTEM+ DPL0+ PRESENT);

	/* GDT Adresse setzen */
	lgdt ();

    /* Interrupts aus */
    disable ();

    /* IDT Adresse setzen */
    lidt ();

    /* init pmode */
    enable_pmode ();

    /* enable Interrupts */
    enable ();

    /*
	** einige Zeit warten ... Timer-Interrupt 8 (und damit Exception 8)
	** wird vor dem Ende der Warteschleife aktiv und fÅhrt damit zum
    ** Aufruf des entsprechenden Exceptionhandlers (==> Exception 8)
	*/
    for (i=0; i<0xFFFF; i++)
		for (j=0; j<0xFFFF; j++) ;

    /* deinit pmode - Wird nicht erreicht !!! */
    disable_pmode ();
	}
