#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
using boost::bind;

class CDivider 
{
public:
	float divide(int x, int y) 
	{
		std::cout << "CDivider::divide" << std::endl; 
		return ((float)x) / y;
	}
};

struct SDivider 
{
	float operator()(int x, int y) const
	{ 
		std::cout << "SDivider::operator()" << std::endl; 
		return ((float)x) / y; 
	};
};

float divide(int x, int y)
{
	std::cout << "divide" << std::endl;
	return ((float)x) / y;
}

template<class F> void printValue(F f)
{
    std::cout << f(5, 3) << std::endl;
}

void testFunctions() 
{
	using namespace std;
	
	boost::function<float (int, int)> f;
	f = SDivider();
	std::cout << f(5, 3) << std::endl;
  
	f = &divide;
	std::cout << f(5, 3) << std::endl;

	CDivider divider;		
	cout << bind(&CDivider::divide, &divider, 5, 3)() << endl;
	
	f = bind(&CDivider::divide, &divider, _1, _2);
	cout << f(5, 3) << endl;
	
	boost::function<float (int)> g;
	g = bind(&CDivider::divide, &divider, _1, 3);
	cout << g(5) << endl;
	
	printValue(f);
}
