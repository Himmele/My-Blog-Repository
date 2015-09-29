// use SharedLib
// HOWTO BUILD: gcc LoadShLib.c

#include <dlfcn.h>

int main()
{
	void (*greetings)();
	int (*get3)();
	void* module;
	Dl_info info;
	
	module = dlopen("/root/SharedLib.so", RTLD_LAZY);		// gain access to an executable object file
																						// RTLD_LAZY: references to functions aren't relocated until that
																						// function is invoked
	
	if(!module)
		exit(0);
		
	greetings = dlsym(module, "greetings");					// get the address of a symbol in a shared object
	get3 = dlsym(module, "get3");									// get the address of a symbol in a shared object
	
	greetings();
	
	printf("get: %d\n", get3());
	
	dladdr(dlsym(module, "get3"), &info);
	printf("library: %s\n", info.dli_fname);
	printf("symbol: %s\n", info.dli_sname);
	
	dlclose(module);
	
	return 0;
}