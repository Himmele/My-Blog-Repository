// 1. gcc -fPIC -c ServerLib.c
// 2. gcc -shared -o ServerLib.so ServerLib.o

int square(int figure)
{
	return figure * figure;
}

int version()
{
	return 1;
}	