#ifndef TEMPLATEPATTERNMATCHING_H_
#define TEMPLATEPATTERNMATCHING_H_

#include <iostream>
using namespace std;

template<typename T>
class TemplatePatternMatching
{
public:
	void type()
	{
		cout << "Object" << endl;
	}
};

template<typename T>
class TemplatePatternMatching<T*>
{
public:
	void type()
	{
		cout << "Pointer" << endl;
	}
};

template<typename T>
class TemplatePatternMatching<T&>
{
public:
	void type()
	{
		cout << "Reference" << endl;
	}
};

template<>
class TemplatePatternMatching<double>
{
public:
	void type()
	{
		cout << "double" << endl;
	}
};

template<>
class TemplatePatternMatching<double*>
{
public:
	void type()
	{
		cout << "double*" << endl;
	}
};

#endif /*TEMPLATEPATTERNMATCHING_H_*/
