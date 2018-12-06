#pragma once

#ifndef __POOL_ALLOCATOR__
#define __POOL_ALLOCATOR__

#include "BaseAllocator.h"
#include "PointerMathUtils.h"

#include <cmath>

namespace Hollow { namespace Core { namespace Memory {

	// Class PoolAlloctor
	class PoolAllocator : public BaseAllocator
	{
	private:
		void** freeList;

		unsigned int objectSize;
		unsigned int objectAlignment;
	public:
		PoolAllocator(void* pMem, unsigned int size, unsigned int objectSize, unsigned int objectAlignment) :
			objectSize(objectSize), objectAlignment(objectAlignment), freeList((void**)pMem),
			BaseAllocator(pMem, size)
		{
			this->clear();
		}

		~PoolAllocator()
		{
			this->clear();
		}

		void* allocate(unsigned int size, unsigned int alignment) override
		{
			if (this->m_MemoryUsed + this->objectSize > this->m_MemorySize) {
				return nullptr;
			}

			void* p = this->freeList;

			this->freeList = (void**)*this->freeList;

			this->m_MemoryUsed += this->objectSize;
			this->m_Allocations++;
			return p;
		}

		void free(void* pMemory) override
		{
			*(void**)pMemory = this->freeList;

			this->freeList = (void**)pMemory;

			this->m_MemoryUsed -= this->objectSize;
			this->m_Allocations--;
		}

		void clear() override
		{
			unsigned short adjustment = GetAdjustment(this->m_MemoryFirstAddress, this->objectAlignment);
			unsigned int objectsCount = floor((this->m_MemorySize - adjustment) / this->objectSize);
			
			this->freeList += adjustment;
			
			void** p = this->freeList;

 			for (unsigned int i = 0; i < objectsCount - 1; i++) 
			{
				*p = (void*)(reinterpret_cast<uintptr_t>(p) + this->objectSize);
				p = (void**)*p;
			}
		}
	};

}}}

#endif