
//          Copyright Oliver Kowalke 2009.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <cstdlib>
#include <iostream>

#include <boost/bind.hpp>
#include <boost/coroutine/all.hpp>

typedef boost::coroutines::coroutine< void() >   coro_t;

void echo( coro_t & ca, int i)
{
	for (int j = 0; j < 2; j++) {
	    std::cout << i; 
    	ca();
	}
}

void runit( coro_t & ca)
{
    std::cout << "started! ";
    for ( int i = 0; i < 10; ++i)
    {
        coro_t c( boost::bind( echo, _1, i) );
        while (c) // returns true as long as c does not exit
            c();
        ca();
    }
}

int main( int argc, char * argv[])
{
    {
        coro_t c( runit);
        while (c) {
            std::cout << "-";
            c();
        }
    }

    std::cout << "\nDone" << std::endl;

    return EXIT_SUCCESS;
}
