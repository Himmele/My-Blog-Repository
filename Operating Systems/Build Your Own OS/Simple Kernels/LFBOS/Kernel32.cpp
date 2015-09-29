extern "C" void PrintASMMessage();
extern "C" void WaitForESC();
extern "C" void LeaveKernel();

void inline __builtin_delete(void*) {};

class Kernel
{
protected:
	unsigned char *LFB;

public:

	Kernel()
	{
		LFB = (unsigned char*) 0xF0000000;
	}

	void PutPixel(unsigned X, unsigned Y, unsigned Color)
	{
		LFB[Y * 4 * 800 + 4 * X + 0] = Color >> 0;
		LFB[Y * 4 * 800 + 4 * X + 1] = Color >> 8;
		LFB[Y * 4 * 800 + 4 * X + 2] = Color >> 16;
		LFB[Y * 4 * 800 + 4 * X + 3] = Color >> 24;
	}

	void DrawLine(unsigned Y, unsigned Color)
	{
		for(int i = 0; i < 800; i++)
			PutPixel(i, Y, Color);
	}

	~Kernel() {}
};

class ExtendedKernel : public Kernel
{
public:

	ExtendedKernel() {}

	~ExtendedKernel() {}
};

int main()
{
	unsigned Color = 0x00113399;
	ExtendedKernel MyKernel;

	for(int i = 0; i < 600; i++)
	{
		MyKernel.DrawLine(i, Color);
		Color += 0x00010101;
	}

	WaitForESC();
	LeaveKernel();
}
