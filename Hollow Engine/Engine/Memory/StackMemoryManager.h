#ifndef __STACK_MEMORY_MANAGER_H__
#define __STACK_MEMORY_MANAGER_H__
#include "Allocators/StackAllocator.h"
#include "Engine/Common/Log.h"
#include <vector>
#include <list>
#define DEFAULT_STACK_MEMORY_MANAGER_SIZE 134217728 // 128 mb

namespace Hollow { namespace Core { namespace Memory {
	class StackMemoryManager
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

#endif