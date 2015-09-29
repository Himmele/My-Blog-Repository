#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/assert.hpp>
#include <boost/intrusive_ptr.hpp>
#include <boost/ref.hpp>
#include <boost/optional.hpp>
#include <boost/fiber/all.hpp>

using namespace boost::fibers;

void ping(unbounded_channel<std::string>& input, unbounded_channel<std::string>& output) {
    fiber::id id(boost::this_fiber::get_id());
	boost::optional<std::string> value;

	while (true) {
		output.put("ping");
		BOOST_ASSERT(input.take(value));
		std::cout <<  id << ": Ping reveiced " << *value << std::endl;
		value.reset();
	}

	output.deactivate();
}

void pong(unbounded_channel<std::string>& input, unbounded_channel<std::string>& output) {
    fiber::id id(boost::this_fiber::get_id());
	boost::optional<std::string> value;

	while (true) {
		BOOST_ASSERT(input.take(value));
		std::cout << id << ": Pong received " << *value << std::endl;
		value.reset();
		output.put("pong");
	}

	output.deactivate();
}

void mainFunc() {
	unbounded_channel<std::string> buffer1, buffer2;
	
	fiber pingFiber(boost::bind(&ping, boost::ref(buffer1), boost::ref(buffer2)));
	fiber pongFiber(boost::bind(&pong, boost::ref(buffer2), boost::ref(buffer1)));

    pingFiber.join();
    pongFiber.join();
}

int main() {
    round_robin scheduling_algorithm;
    detail::scheduler::replace(&scheduling_algorithm);
	try {
		fiber mainFiber(mainFunc);
        while (mainFiber) { // Loop while main fiber has not finished
			std::cout << "Scheduling next fiber..." << std::endl;
			boost::fibers::run();
		}

		return EXIT_SUCCESS;
	}
	catch ( std::exception const& e)
	{ std::cerr << "exception: " << e.what() << std::endl; }
	catch (...)
	{ std::cerr << "unhandled exception" << std::endl; }
	return EXIT_FAILURE;
}
