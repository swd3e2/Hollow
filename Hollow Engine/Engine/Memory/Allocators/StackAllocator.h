#pragma once

#ifndef __STACK_ALLOCATOR__
#define __STACK_ALLOCATOR__
#include "BaseAllocator.h"
#include "PointerMathUtils.h"

namespace Hollow { namespace Core { namespace Memory {

	// Allocators base class describing the allocator interface. 
	class LinearAllocator : public BaseAllocator
	{
		struct AllocatorHeader {
			unsigned int size;
		};

	public:
		LinearAllocator(void* pMem, unsigned int size) :
			BaseAllocator(pMem, size)
		{}

		virtual ~LinearAllocator() 
		{
			this->clear();
		}

		virtual void* allocate(unsigned int size, unsigned int alignment)
		{
			if (this->m_MemoryUsed + size > this->m_MemorySize) 
			{
				return nullptr;
			}
			unsigned char adjustment = GetAdjustmentWithHeader();

			void* p = (void*)(reinterpret_cast<uintptr_t>(this->m_MemoryFirstAddress) + this->m_MemoryUsed + size);

			this->m_Allocations++;
			this->m_MemoryUsed += size;

			return p;
		}

		virtual void free(void* pMmem)
		{
			this->m_Allocations--;
			this->m_MemoryUsed;
		}

		virtual void clear()
		{
			this->m_Allocations = 0;
			this->m_MemoryUsed = 0;
		}

	};

}}}


#endif