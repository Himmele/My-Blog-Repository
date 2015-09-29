#include <boost/context/all.hpp>
#include <iostream>

#define MIN_STACK_SIZE 8 * 1024
#define DEFAULT_STACK_SIZE 64 * 1024
#define MAX_STACK_SIZE 256 * 1024

using namespace boost::context;
 
fcontext_t mainContext;
fcontext_t* helloContext;
fcontext_t* worldContext;
 
void helloTask(intptr_t args) {
    while (true) {
        std::cout << "Hello ";
     	jump_fcontext(helloContext, &mainContext, 0);
    }
}

void worldTask(intptr_t args) {
    while (true) {
        std::cout << "World" << std::endl;
     	jump_fcontext(worldContext, &mainContext, 0);
    }
}
 
int main() {
	typedef simple_stack_allocator<MAX_STACK_SIZE, DEFAULT_STACK_SIZE, MIN_STACK_SIZE> stack_allocator;
    stack_allocator allocator;
 
    void* sp = allocator.allocate(stack_allocator::default_stacksize());
    helloContext = make_fcontext(sp, stack_allocator::default_stacksize(), helloTask);
 
    sp = allocator.allocate(stack_allocator::default_stacksize());
    worldContext = make_fcontext(sp, stack_allocator::default_stacksize(), worldTask);
 
    for (int i = 0; i < 10; i++) {
        jump_fcontext(&mainContext, helloContext, 0);
        jump_fcontext(&mainContext, worldContext, 0);
    }
    
	return 0;
}

