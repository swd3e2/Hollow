#include "StackMemoryManager.h"

namespace Hollow { namespace Core { namespace Memory {

	StackMemoryManager::StackMemoryManager()
	{
		this->m_GlobalMemory = malloc(DEFAULT_STACK_MEMORY_MANAGER_SIZE);
		this->m_Allocator = new StackAllocator(this->m_GlobalMemory, DEFAULT_STACK_MEMORY_MANAGER_SIZE);
		HW_DEBUG("StackMemoryManager: created, firstAddress {}, size {}", this->m_GlobalMemory, DEFAULT_STACK_MEMORY_MANAGER_SIZE);
	}

	StackMemoryManager::~StackMemoryManager()
	{
		free(m_GlobalMemory);
	}

	void* StackMemoryManager::allocate(unsigned int size, const char* user)
	{
		void* mem = m_Allocator->allocate(size, alignof(unsigned char));
		m_UsedMemory.push_back({ user, mem });
		HW_INFO("StackMemoryManager: allocated memory with size {}, pointer {}, user {}", size, mem, user);

		return mem;
	}

	void StackMemoryManager::free(void* mem)
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

}}}