#include <iostream>
#include <boost/function.hpp>
#include <functional>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <vector>
using boost::bind;

void testAlgorithms()
{
	std::vector<int> v;
	v.push_back(111);
	v.push_back(107);
	v.push_back(101);
	v.push_back(90);
	v.push_back(106);

	int upperBound = 103;
	/* std::less(x,y) returns true if x < y and false otherwise */
	/* find_if() searches for the first element for which the unary predicate up (std::less in this example ) returns true */
	/* the iterator pointing to that element is returned if found */
	std::cout << "The first Element less than " << upperBound << " is: " 
		<< *find_if(v.begin(), v.end(), bind(std::less<int>(), _1, upperBound)) << std::endl; 
}
