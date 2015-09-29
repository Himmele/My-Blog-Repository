#include <sys/neutrino.h>
#include <inttypes.h>

int main()
{
	uint64_t sleep = 5000000000;			// 5 seconds
	
	printf("going to sleep for 5 seconds\n");
	TimerTimeout(CLOCK_REALTIME, _NTO_TIMEOUT_NANOSLEEP, NULL, &sleep, NULL);
	printf("awakened\n");
	
	return 0;
}