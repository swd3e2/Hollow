#pragma once
#include <cstddef>
#include <vector>
#include <list>
#include "Allocators/StackAllocator.h"

#define ECS_GLOBAL_MEMORY_CAPACITY 134217728 // 128 MB

namespace ECS { namespace Memory {

	class MemoryManager
	{
		using StackAllocator = Allocators::StackAllocator;
	public:
		static constexpr std::size_t MEMORY_CAPACITY = ECS_GLOBAL_MEMORY_CAPACITY;
	private:
		void*										m_GlobalMemory;
		StackAllocator*								m_MemoryAllocator;
		std::vector<std::pair<const char*, void*>>	m_PendingMemory;
		std::list<void*>							m_FreedMemory;

		MemoryManager(const MemoryManager&) = delete;
		MemoryManager& operator=(MemoryManager&) = delete;
	public:
		MemoryManager();
		~MemoryManager();

		inline void* Allocate(std::size_t memSize, const char* user = nullptr)
		{
			void* pMemory = m_MemoryAllocator->allocate(memSize, alignof(u8));

			this->m_PendingMemory.push_back(std::pair<const char*, void*>(user, pMemory));

			return pMemory;
		}

		inline void Free(void* pMem)
		{
			if (pMem == this->m_PendingMemory.back().second)
			{
				this->m_MemoryAllocator->free(pMem);
				this->m_PendingMemory.pop_back();

				bool bCheck = true;
				while (bCheck == true)
				{
					bCheck = false;

					// do not report already freed memory blocks.
					for (auto it : this->m_FreedMemory)
					{
						if (it == this->m_PendingMemory.back().second)
						{
							this->m_MemoryAllocator->free(it);
							this->m_PendingMemory.pop_back();
							this->m_FreedMemory.remove(it);

							bCheck = true;
							break;
						}
					}
				};
			}
			else
			{
				this->m_FreedMemory.push_back(pMem);
			}
		}
	};

}}