#ifndef DLL2_H_
#define DLL2_H_

class DLL1;

class DLL2
{
public:
	DLL2();
	~DLL2();
	void print();

private:
	DLL1* dll1;
};

#endif /*DLL2_H_*/
