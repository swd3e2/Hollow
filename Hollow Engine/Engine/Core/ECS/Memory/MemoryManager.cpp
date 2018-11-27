#include "MemoryManager.h"

namespace ECS { namespace Memory {

	MemoryManager::MemoryManager()
	{
		// allocate global memory
		this->m_GlobalMemory = malloc(MemoryManager::MEMORY_CAPACITY);

		// create allocator
		this->m_MemoryAllocator = new Allocators::StackAllocator(MemoryManager::MEMORY_CAPACITY, this->m_GlobalMemory);

		this->m_PendingMemory.clear();
		this->m_FreedMemory.clear();
	}

	MemoryManager::~MemoryManager()
	{
		this->m_MemoryAllocator->clear();

		delete this->m_MemoryAllocator;
		this->m_MemoryAllocator = nullptr;

		free(this->m_GlobalMemory);
		this->m_GlobalMemory = nullptr;
	}

}}