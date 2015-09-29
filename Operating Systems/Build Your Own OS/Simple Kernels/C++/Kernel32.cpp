extern "C" void PrintASMMessage();
extern "C" void LeaveKernel();

void inline __builtin_delete(void*) {};

class Kernel
{
protected:
	char Message[128];
	unsigned short *VideoMemory;
	
public:

	Kernel() 
	{
		VideoMemory = (unsigned short*) 0xB8000;
		SetUpMessage("Hello World from a C++ Kernel");
		
		PrintASMMessage();
	}
	
	~Kernel() {}
	
	void SetUpMessage(char* String)
	{
		char* Position;
		int Index = 0;

		for(Position = String; *Position; Position++, Index++)
			Message[Index] = String[Index];
		
		Message[Index] = '\0';
	}
		
	void PrintFromCPPKernel(unsigned Line)
	{	
		char* Position;
		int Index = 0;
		
		for (Position = Message; *Position; Position++, Index++)
			VideoMemory[Index + Line * 80] = (unsigned char) *Position | 0x0700;
	}
	
	virtual void VirtualFunction() {}
};

class ExtendedKernel : public Kernel
{
public:
	
	ExtendedKernel() {}
	
	~ExtendedKernel() {}

	virtual void VirtualFunction()
	{		
		SetUpMessage("Virtual Functions in a C++ Kernel");
		PrintFromCPPKernel(2);
	}
};

int main()
{
	ExtendedKernel MyFirstCPPKernel;
	Kernel* MyKernel;
	
	MyKernel = &MyFirstCPPKernel;
	MyFirstCPPKernel.PrintFromCPPKernel(1);
	MyKernel->VirtualFunction();
	LeaveKernel();
}
