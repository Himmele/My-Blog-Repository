#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class Object
{
public:
	Object(int value)
	{
		this->value = value;
	}
	
	~Object()
	{
	}
	
private:
	int value;
};

struct B;
struct A
{
	int value;
	boost::shared_ptr<B> b;
};

struct B
{
	int value;
	boost::weak_ptr<A> a;
};

void testSmartPointers()
{
	boost::shared_ptr<int> pInt( new int(2) );
	std::cout << "pInt: " << *pInt << std::endl;
	
	boost::shared_ptr<Object> pObject( new Object(4) );
	
	boost::shared_ptr<A> pA;
	boost::shared_ptr<B> pB;
	{
		pA.reset( new A() );
		pB.reset( new B() );
		pA->value = 4;
		/* A -> B */
		pA->b = pB;
		pB->value = 8;
		/* B -> A */
		//pB->a = pA;
	}
	std::cout << "Value of pA: " << pA->value << std::endl;
	std::cout << "Value of pB: " << pB->value << std::endl;
	boost::shared_ptr<A> pA2= pB->a.lock();
	if(pA2.get() != 0L)
	{
		std::cout << "Value of pA: " << pA2->value << std::endl;
	}
	std::cout << "Value of pB: " << pA->b->value << std::endl;
	std::cout << "Refs to pA: " << pA.use_count() << std::endl;
	std::cout << "Refs to pB: " << pB.use_count() << std::endl;
}
