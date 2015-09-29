#include "TemplatePatternMatching.h"

int main()
{
	TemplatePatternMatching<int> t1;
	t1.type();
	
	TemplatePatternMatching<int*> t2;
	t2.type();
	
	TemplatePatternMatching<int&> t3;
	t3.type();
	
	TemplatePatternMatching<double> t4;
	t4.type();
	
	TemplatePatternMatching<double*> t5;
	t5.type();
	
	return 0;
}