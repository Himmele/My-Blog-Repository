#include <sys/neutrino.h>


int main()
{
	int runmask = 1;
	
	/* the spinlock structure consists of a volatile unsigned int value */
	/* 
	* VOLATILE: der Compiler muss davon ausgehen, dass sich der Wert der Variablen durch Einflüsse von außen *
	* (z. B. ISR, microkernel, ...) ändert. Daher verzichtet er auf Optimierungoperationen, die u. a. darin bestünden, *
	* den Wert der Variablen nicht bei jedem Zugriff auf die Variable erneut aus dem Speicher in ein Prozessorregister *
	* zu laden, sondern dies vielleicht nur einmal zu tun und danach stets diese Kopie des Variablenwerts für weitere *
	* Operationen zu verwenden *
	*/
	static intrspin_t spinlock;
	
	printf("spinlock: %d\n", spinlock.value);
	
	ThreadCtl( _NTO_TCTL_IO, 0 );
	
	if(ThreadCtl(_NTO_TCTL_RUNMASK, &runmask) == -1)
		printf("no SMP system\n");
	
	// spinlock.value = 1;													// this brings the machine down: the InterruptLock routine locks all interrupts
																						// and the memory bus trying to aquire the spinlock
	InterruptLock(&spinlock);												// guard a critical section in an interrupt handler even in a SMP system
	printf("guarded against any preemption\n");
	InterruptUnlock(&spinlock);
	
	return 0;
}