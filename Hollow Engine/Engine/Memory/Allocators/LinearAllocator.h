#pragma once

#ifndef __LINEAR_ALLOCATOR__
#define __LINEAR_ALLOCATOR__
#include "BaseAllocator.h"
#include "PointerMathUtils.h"

namespace Hollow { namespace Core { namespace Memory {

	// Class LinearAlloctor
	class LinearAllocator : public BaseAllocator
	{
	public:
		LinearAllocator(void* pMem, unsigned int size) :
			BaseAllocator(pMem, size)
		{}
		
		~LinearAllocator()
		{
			this->clear();
		}

		void* allocate(unsigned int size, unsigned int alignment) override
		{
			assert(size < this->m_MemorySize);

			union
			{
				uintptr_t asUIntPtr;
				void* asVoidPtr;
			};

			asVoidPtr = (void*)this->m_MemoryFirstAddress;
			asUIntPtr += this->m_MemoryUsed;

			unsigned char adjustment = GetAdjustment(asVoidPtr, alignment);

			// Out of memory
			if (this->m_MemoryUsed + size + adjustment > this->m_MemorySize)
				return nullptr;
			
			this->m_MemoryUsed += size + adjustment;
			this->m_Allocations++;

			return asVoidPtr;
		}

		void free(void* pMemory) override
		{
			assert(false);
		}

		void clear() override
		{
			this->m_MemoryUsed = 0;
			this->m_Allocations = 0;
		}
	};

}}}

#endif