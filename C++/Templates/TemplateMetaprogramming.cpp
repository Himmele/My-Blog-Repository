#include <iostream>

template<long B, unsigned long E>
struct PowHelper
{
	static const double value;
};

template<long B, unsigned long E>
const double PowHelper<B, E>::value = B * PowHelper<B, E - 1>::value;

template<long B>
struct PowHelper<B, 0>
{
	static const double value;
};

template<long B>
const double PowHelper<B, 0>::value = 1;

template<long B,long E>
struct Power
{
	static const double value;
};

template<long B, long E>
const double Power<B, E>::value = E < 0 ? 1.0 / PowHelper<B, E>::value : PowHelper<B, E>::value;


int main()
{
 	std::cout << Power<2, 5>::value << std::endl;

 	return 0;
}
