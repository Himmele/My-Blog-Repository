#include "DLL2.h"
#include "DLL1.h"
#include <stdio.h>

DLL2::DLL2()
{
	printf("DLL2::DLL2\n");
	dll1 = new DLL1();
}

DLL2::~DLL2()
{
	printf("DLL2::~DLL2\n");
	delete dll1;
}

void DLL2::print()
{
	printf("DLL2::print\n");
	dll1->print();
}
