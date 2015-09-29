/*
** PM_03.C - Protected Mode unter C (BC 4.0 !)
**
** Erste Version:     26.08.97
** Letze 始derungen:  31.08.97
**
** (c) Copyright 1997 by Jens Hohmuth. All rights reserved.
** email: Jens.Hohmuth@fh-zwickau.de
*/

// Konvertierung von linearen (Base 0) Adressen in relative (Makro lin2rel)
// Beispiel Videosegment: char *video= (char*) lin2rel (0xB8000);
extern  int DS_START;
#define lin2rel(src) (src-DS_START)

// Da keine Bilbliotheksfunktionen genutzt werden k馬nen (WIN32),
// m《sen entsprechende Funktionen bereitgestellt werden.
// strlen zur Ermittlung der L�nge eines NULL-terminierten Strings
int strlen (char *s)
    {
    int i= 0;

    while (s[i] != '\0') i++;

    return i;
    }

/*
** print - Stringausgabe in Videosegment
**
** In:  x - Ausgabeposition X  
**      y - Ausgabeposition Y
**      s - String
**      a - Attributsbyte (-1 f〉 keine 始derung)
** Out: nil
*/
void print (int x, int y, char *s, int a)
    {
    char *video;
    int i;

    video= (char*) lin2rel (0xB8000+ y*160+ x*2);

    if (a == -1)
        {
        // keine 始derung der Attributwerte
        for (i=0; i<strlen(s); i++)
            {
            *(video+ 0)= s[i];
            video+= 2;
            }
        }
    else
        {
        // Mit 始derung der Attributwerte
        for (i=0; i<strlen(s); i++)
            {
            *(video++)= s[i];
            *(video++)= (char) a;
            }
        }
    }

void main (void)
	{
    print (10, 10, "Hallo Welt vom Protected Mode aus BC 4.0 !!!", 0x0a);
	}
