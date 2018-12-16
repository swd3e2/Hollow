#pragma once

#ifndef __MEMORY_USER__
#define __MEMORY_USER__
#include "StackMemoryManager.h"

namespace Hollow { namespace Core { namespace Memory {

	class MemoryUser
	{
	protected:
		StackMemoryManager* memoryManager;
	public:
		MemoryUser()
		{
			memoryManager = new StackMemoryManager();
		}

		void* allocate(unsigned int memSize, const char* user = nullptr)
		{
			return this->memoryManager->allocate(memSize, user);
		}

		void free(void* mem)
		{
			this->memoryManager->free(mem);
		}
	};

}}}
#endif