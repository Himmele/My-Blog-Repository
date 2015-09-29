// shared library
// HOWTO BUILD:
// 1. gcc -fPIC -c SharedLib.c --> fPIC: no absolute adressing, whenever it is possible; c: compile and assemble, but not link
// 2. gcc -shared -o SharedLib.so SharedLib.o --> shared: link all libraries together

void greetings()
{
	printf("Hello!\n");
}

int get3()
{
	return 3;
}