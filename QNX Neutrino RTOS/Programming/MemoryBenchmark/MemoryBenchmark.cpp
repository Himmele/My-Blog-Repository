#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syspage.h>

int main(int argc, char *argv[])
{
	const unsigned int SIZE = 10 * 1024 * 1024;
	const unsigned int ITERATIONS = 40;
	char* src = new char[SIZE];
	char* dst = new char[SIZE];
	memset(src, 0x00, SIZE);
	clock_t startTime, endTime;
	startTime = clock();
	for(unsigned int i = 0; i < ITERATIONS; i++)
	{
		memcpy(dst, src, SIZE);
	}
	endTime = clock();
	float time = (float)((endTime - startTime)) / (CLOCKS_PER_SEC / 1000.0);
	printf("Time: %.0f ms\n", time);
	printf("Data Rate: %.02f MB/sec\n", (((ITERATIONS * SIZE) / time) * 1000.0) / (1024 * 1024));
	delete src;
	delete dst;

	return EXIT_SUCCESS;
}
