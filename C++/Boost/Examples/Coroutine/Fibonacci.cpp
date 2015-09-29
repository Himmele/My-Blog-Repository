// Iterator example (--> infinite list)

#include <cstdlib>
#include <iostream>
#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/coroutine/all.hpp>

using namespace boost;
using namespace boost::coroutines;

void fibonacci(coroutine<int()>::caller_type& caller) {
    int a = 1, b = 1;
    while (true) {
        int c = a + b;
        a = b;
        b = c;
        caller(c); // Yield the value of c to the caller context --> Python generators
    }
}

int main() {
	coroutine<int()> callee(fibonacci); // Coroutine with int result type
	range_iterator< coroutine<int()> >::type itr(boost::begin(callee));
    BOOST_ASSERT(boost::end(callee) != itr);

    std::cout << "1, 1, ";
    for (int i = 0; i < 10; i++) {
        std::cout << *itr <<  ", ";
        ++itr;
    }

    std::cout << std::endl;

    return 0;
}
