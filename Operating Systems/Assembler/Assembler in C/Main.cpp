#include <stdio.h>

short addition(short, short);

void main()
{
	short Summe, Erster_Summand, Zweiter_Summand;
	printf("Erster Summand: ");
	scanf("%d", &Erster_Summand);
	printf("Zweiter Summand: ");
	scanf("%d", &Zweiter_Summand);
	
	Summe = addition(Erster_Summand, Zweiter_Summand);

	printf("Summe: %d\n", Summe);
}


short addition(short a, short b)
{
	__asm mov eax,[ebp+8]
	__asm add eax,[ebp+12]
}