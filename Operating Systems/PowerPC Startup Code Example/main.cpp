extern char __BSS_START__, __BSS_END__;
extern char __TEXT_END__, __DATA_START__, __DATA_END__;

extern void (*__CTOR_LIST__[])(void);
extern void (*__CTOR_END__[]) (void);

extern "C" unsigned long atomic(unsigned long *value, unsigned long increment);

// used for threadsafe statics
/* namespace __cxxabiv1
{
	extern "C"
	{
		int __cxa_guard_acquire(int *guard)
		{
			// get semaphore --> e. g. BIT TEST AND SET

			// if not yet constructed
				// initialisiere statische/s Variable bzw. Objekt
				return 1;
			// else
				// KEINE Initialisierung
				return 0;
		}

		void __cxa_guard_release(int *guard)
		{
			// release semaphore --> e. g. BIT TEST AND SET
		}
	}
}; */

extern "C" int atexit(void (*dtor)(void))
{
	// call destructors for non-global static objects

	return 0;
}

extern "C"
{
	void initRAM();
	void _start();
	void __eabi();
	void __eabi_ctors();
	int main();
}

extern "C"
{
	void* memcpy (void* dest, const void* source, unsigned int size)
	{
		for(unsigned int i = 0; i < size; i++)
		{
			(static_cast<unsigned char*>(dest))[i] = (static_cast<const unsigned char*>(source))[i];
		}
		return dest;
	}
}

void initRAM()
{
	char *src;
	char *dst;

	/* initialize BSS */
	for(dst = &__BSS_START__; dst < &__BSS_END__; dst++)
		*dst = 0;

	src = &__TEXT_END__;
	dst = &__DATA_START__;

	/* copy .data from ROM to RAM */
	while(dst < &__DATA_END__)
		*dst++ = *src++;

	_start();
}

void __eabi()
{
	__eabi_ctors();
}

void _start()
{
	main();
}

void __eabi_ctors()
{
	int ctor;
	int last = __CTOR_END__ - __CTOR_LIST__;

	for(ctor = 0; ctor < last; ctor++)
		(*(__CTOR_LIST__[ctor]))();
}

int toBSS;
int toDATA = 7;

__attribute__((section(".vectors"))) char interruptHandlers[0x2100];



class A
{
	public:

		A(int i) : i(i)
		{
		}

		~A()
		{
		}

		void inc()
		{
			i++;
		}

		int get()
		{
			return i;
		}

	private:
		int i;
};

A a(1);
A b(2);
A c(3);
A *ptrA;

A* getNewA()
{
	// GCC calls __cxa_guard_acquire here --> if threadsafe-statics enabled
	static A d(4);
	// GCC calls __cxa_guard_release here --> if threadsafe-statics enabled
	// GCC calls atexit here

	return &d;
}

class StackCopy
{
public:
	StackCopy(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m, int n, int o, int p, int q, int r, int s, int t, int u, int v, int w, int x, int y, int z)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
		this->e = e;
		this->f = f;
		this->g = g;
		this->h = h;
		this->i = i;
		this->j = j;
		this->k = k;
		this->l = l;
		this->m = m;
		this->n = n;
		this->o = o;
		this->p = p;
		this->q = q;
		this->r = r;
		this->s = s;
		this->t = t;
		this->u = u;
		this->v = v;
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}

	int a;
	int b;
	int c;
	int d;
	int e;
	int f;
	int g;
	int h;
	int i;
	int j;
	int k;
	int l;
	int m;
	int n;
	int o;
	int p;
	int q;
	int r;
	int s;
	int t;
	int u;
	int v;
	int w;
	int x;
	int y;
	int z;
};

StackCopy getStackCopy()
{
	return StackCopy(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26);
}

class RegCopy
{
public:
	RegCopy(int a, int b, int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}

	int a;
	int b;
	int c;
};

RegCopy getRegCopy()
{
	return RegCopy(0xAA, 0xBB, 0xCC);
}

int return4()
{
	return 4;
}

class B
{
public:
	B(int a, int b)
	{
		this->a = a;
		this->b = b;
	}

	int a;
	int b;
};

B getB()
{
	return B(0xAABBCCDD, 0x11223344);
}

int main()
{
	unsigned long atomicValue = 4;
	unsigned long retValue = atomic(&atomicValue, 7);

	A fourAs[4] = {1, 2, 3, 4};

	a.inc();
	a.get();

	ptrA = getNewA();
	ptrA = getNewA();

	ptrA->inc();

	toBSS++;

	/* StackCopy Objekt wird aus lokalem Stack-Frame von getStackCopy in den aktuellen Stack-Frame kopiert */
	/* ab 3 Rückgabewerte erfolgt die Rückgabe über den Stack und nicht mehr über Register */
	/* ab dem 8. Parameter werden diese ebenfalls über den Stack übergeben und nicht mehr ausschließlich über Register */
	const StackCopy& stackCopy = getStackCopy();
	/* Error --> Veränderung eines temporären Objekts ist fehleranfällig und deshalb verboten */
	// StackCopy& stackCopy = getStackCopy();
	const RegCopy& regCopy = getRegCopy();
	int ret4 = return4();
	B b = getB();

	int terminator = 0x1234567;

	return 0;
}
