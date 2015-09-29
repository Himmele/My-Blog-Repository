// use SharedLib
// HOWTO BUILD:
// 1. gcc -c DynLinked.c --> compile and assemble, but not link
// 2. gcc -o DynLinked DynLinked.o /root/SharedLib.so

#include <dlfcn.h>

int main()
{
	void (*greet)();
	int (*printit)(const char*, ...);
	
	greetings();
	printf("get: %d\n", get3());
	
	greet = dlsym(RTLD_DEFAULT, "greetings");		// search all objects for the given symbol
	greet();
	
	printit = dlsym(RTLD_DEFAULT, "printf");			// from libc
	printit("libc\n");
	
	return 0;
}