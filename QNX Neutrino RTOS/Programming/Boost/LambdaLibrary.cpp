#include <iostream>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
//#include <boost/bind.hpp>
#include <boost/lambda/algorithm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/ref.hpp>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <list>

void testLambdaLibrary()
{
	using namespace boost::lambda;
  	
  	std::vector<int> v(10);  	
	std::for_each(v.begin(), v.end(), _1 = 42);	
	std::for_each(v.begin(), v.end(), std::cout << _1 << ' ');
	std::cout<< std::endl;
	for(unsigned int i = 0; i < v.size(); i++ )
	{
   		v[i] = i;
 	}
 	std::for_each(v.begin(), v.end(), std::cout << _1 << ' ');
	std::cout<< std::endl;
	std::sort(v.begin(), v.end(), _1 > _2);
	std::for_each(v.begin(), v.end(), std::cout << _1 << ' ');
	std::cout<< std::endl;
	
	boost::shared_ptr<std::string> pString( new std::string("Test") );
	std::cout << *pString << std::endl;
	std::for_each(pString->begin(), pString->end(), _1 = '*');	
	std::cout << *pString << std::endl;
}
