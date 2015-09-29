#include "MemoryManager.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>

using namespace std;

MemoryManager::MemoryManager(unsigned int memorySize)
{
	this->memorySize = memorySize;
	shm_unlink( "/MemoryManager" );    
	fd = shm_open( "/MemoryManager", O_RDWR | O_CREAT, 0 );
	if(fd >= 0)
	{		
  		if(shm_ctl( fd, SHMCTL_PHYS | SHMCTL_GLOBAL | SHMCTL_ANON, 0, memorySize ) == 0 )
  		{
  			virtBaseAddress = mmap( 0, memorySize, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0  );  
			if(virtBaseAddress == MAP_FAILED )
			{
				printf("Error: mmap\n");
				exit(1);
			}			
  		}
  		else
  		{
  			printf("Error: shm_ctl\n");
			exit(1);
  		}
	}
	else
	{
		printf("Error: shm_open\n");
		exit(1);
	}
	freeMemory.push_back(MemoryNode(reinterpret_cast<unsigned int>(virtBaseAddress), memorySize));
}

MemoryManager::~MemoryManager()
{
	close( fd );
    shm_unlink( "/MemoryManager" );
}

void* MemoryManager::allocate(unsigned int size)
{
	if(size != 0)
	{	
		list<MemoryNode>::iterator itr;
		for(itr = freeMemory.begin(); itr != freeMemory.end(); ++itr)
		{
			// 4 Byte Alignment
			unsigned int allocSize = ((size + 3) & 0xFFFFFFFC);			
			if(itr->size >= allocSize)
			{
				void* allocMemory = reinterpret_cast<void*>(itr->address);
				usedMemory.push_back(MemoryNode(itr->address, allocSize));
				itr->address += allocSize;
				itr->size -= allocSize;
				return allocMemory;
			}
		}
	}
	return 0L;
}

void MemoryManager::free(void* memory)
{	
	list<MemoryNode>::iterator itr;
	for(itr = usedMemory.begin(); itr != usedMemory.end(); ++itr)
	{
		if(itr->address == reinterpret_cast<unsigned int>(memory))
		{
			deallocate(itr);			
			break;
		}
	}
}

void MemoryManager::deallocate(list<MemoryNode>::iterator node)
{
	list<MemoryNode>::iterator predecessor = freeMemory.end();
	list<MemoryNode>::iterator successor = freeMemory.end();	
	for(list<MemoryNode>::iterator itr = freeMemory.begin(); itr != freeMemory.end(); ++itr)
	{
		if(itr->address + itr->size == node->address)
		{			
			predecessor = itr;
			break;
		}
	}
	for(list<MemoryNode>::iterator itr = freeMemory.begin(); itr != freeMemory.end(); ++itr)
	{
		if(node->address + node->size == itr->address)
		{
			successor = itr;
			break;
		}
	}
	if((predecessor != freeMemory.end()) && (successor != freeMemory.end()))
	{		
		predecessor->size += (node->size + successor->size);
		freeMemory.erase(successor);
	}
	else if(predecessor != freeMemory.end())
	{		
		predecessor->size += node->size;
	}
	else if(successor != freeMemory.end())
	{		
		successor->address = node->address;
		successor->size += node->size;
	}
	else
	{	
		freeMemory.push_back(*node);
	}
	usedMemory.erase(node);
}

void MemoryManager::print()
{
	list<MemoryNode>::iterator itr;
	printf("Free Memory: ");
	for(itr = freeMemory.begin(); itr != freeMemory.end(); ++itr)
	{
		printf("%08X:%d -> ", itr->address, itr->size);
	}
	printf("\n");
	printf("Used Memory: ");
	for(itr = usedMemory.begin(); itr != usedMemory.end(); ++itr)
	{
		printf("%08X:%d -> ", itr->address, itr->size);
	}
	printf("\n"); 
}

int main()
{
	MemoryManager* memoryManager = new MemoryManager(64 * 1024 * 1024);
	memoryManager->print();
	void* chunk1 = memoryManager->allocate(1);
	memoryManager->print();
	void* chunk2 = memoryManager->allocate(1);
	memoryManager->print();
	void* chunk3 = memoryManager->allocate(1);
	memoryManager->print();
	void* chunk4 = memoryManager->allocate(1);
	memoryManager->print();
	printf("\n");
	memoryManager->free(chunk2);
	memoryManager->print();
	memoryManager->free(chunk3);
	memoryManager->print();
	memoryManager->free(chunk4);
	memoryManager->print();
	memoryManager->free(chunk1);
	memoryManager->print();	
	if(memoryManager->allocate(0) == 0L)
	{
		printf("OK\n");
	}
	delete memoryManager;
	
	return 0;
}
