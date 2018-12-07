#pragma once

#ifndef __STACK_MEMORY_MANAGER__
#define __STACK_MEMORY_MANAGER__
#include <vector>
#include <list>
#include "Allocators/StackAllocator.h"

#define DEFAULT_STACK_MEMORY_MANAGER_SIZE 134217728 // 128 mb

namespace Hollow { namespace Core { namespace Memory {

	class StackMemoryManager
	{
	private:
		StackAllocator *							m_Allocator;
		std::vector<std::pair<const char*, void*>>	m_UsedMemory;
		std::list<void*>							m_FreedMemory;
		void*										m_GlobalMemory;
		
		StackMemoryManager(const StackMemoryManager&) = delete;
		StackMemoryManager& operator=(StackMemoryManager&) = delete;
	public:
		StackMemoryManager()
		{
			this->m_GlobalMemory = malloc(DEFAULT_STACK_MEMORY_MANAGER_SIZE);
			this->m_Allocator = new StackAllocator(this->m_GlobalMemory, DEFAULT_STACK_MEMORY_MANAGER_SIZE);
		}

		~StackMemoryManager()
		{
			free(m_GlobalMemory);
		}

		inline void* allocate(unsigned int size, const char* user)
		{
			void* mem = m_Allocator->allocate(size, alignof(unsigned char));
			m_UsedMemory.push_back({ user, mem });

			return mem;
		}

		void free(void* mem)
		{
			if (m_UsedMemory.back().second == mem)
			{
				this->m_Allocator->free(mem);
				this->m_UsedMemory.pop_back();

				bool check = true;
				while (check == true)
				{
					check = false;

					for (auto it : this->m_FreedMemory)
					{
						if (it == this->m_UsedMemory.back().second)
						{
							this->m_Allocator->free(it);
							this->m_UsedMemory.pop_back();
							this->m_FreedMemory.remove(it);

							check = true;
							break;
						}
					}
				}
			}
			else
			{
				this->m_FreedMemory.push_back(mem);
			}
		}
	};

}}}

#endif