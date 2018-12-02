#pragma once
#include "Allocators/StackAllocator.h"
#include "MemoryManager.h"

namespace ECS { namespace Memory {

	class GlobalMemoryUser
	{
	private:
		MemoryManager * memoryManager;
	public:
		GlobalMemoryUser() : memoryManager(new MemoryManager()) {}

		virtual ~GlobalMemoryUser() {}

		inline void* Allocate(std::size_t memSize, const char * user = nullptr)
		{
			return this->memoryManager->Allocate(memSize, user);
		}

		inline void Free(void* mem)
		{
			this->memoryManager->Free(mem);
		}
	};

}}