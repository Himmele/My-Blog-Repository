// 1. gcc -c Client.c
// 2. gcc -o Client Client.o /root/ClientStub.so

#include <stdio.h>

int main()
{
	Initialize();
	
	printf("Shared library version: %d\n", GetVersion());
	
	printf("Square of 5: %d\n", GetSquare(5));
	printf("Square of 9: %d\n", GetSquare(9));
}