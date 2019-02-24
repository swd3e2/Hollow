#pragma once
#include "Allocators/StackAllocator.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include <vector>
#include <list>
#define DEFAULT_STACK_MEMORY_MANAGER_SIZE 67108864 // 64 mb

namespace Hollow { namespace Core { namespace Memory {
	class HOLLOW_API StackMemoryManager
	{
	private:
		std::vector<std::pair<const char*, void*>>	m_UsedMemory;
		std::list<void*>							m_FreedMemory;
		void*										m_GlobalMemory;
		
		StackMemoryManager(const StackMemoryManager&) = delete;
		StackMemoryManager& operator=(StackMemoryManager&) = delete;
	protected:
		StackAllocator *							m_Allocator;
	public:
		StackMemoryManager();
		~StackMemoryManager();
		void* allocate(unsigned int size, const char* user);
		void free(void* mem);
	};

}}}