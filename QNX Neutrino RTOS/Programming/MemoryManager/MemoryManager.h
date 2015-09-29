#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <list>

class MemoryManager
{
public:
	struct MemoryNode
	{
		MemoryNode(unsigned int address, unsigned int size)
		{		
			this->address = address;
			this->size = size;		
		}		
		unsigned int address;
		unsigned int size;
	};
	
	MemoryManager(unsigned int memorySize);
	virtual ~MemoryManager();
	void* allocate(unsigned int size);
	void free(void* memory);
	void print();	
	
private:
	void deallocate(std::list<MemoryNode>::iterator node);
	
	int fd;
	void* virtBaseAddress;
	unsigned int memorySize;	
	std::list<MemoryNode> freeMemory;
	std::list<MemoryNode> usedMemory;
};

#endif /*MEMORYMANAGER_H_*/
