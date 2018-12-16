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
		{
			this->clear();
			Hollow::Log::GetCoreLogger()->debug("LinearAllocator: created, firstAddress {}, size {}", this->GetMemoryFirstAddress(), this->GetMemorySize());
		}
		
		~LinearAllocator()
		{
			this->clear();
		}

		void* allocate(unsigned int size, unsigned int alignment) override
		{
			void* p = (void*)(reinterpret_cast<size_t>(this->m_MemoryFirstAddress) + this->m_MemoryUsed);

			unsigned char adjustment = GetAdjustment(p, alignment);

			// Out of memory
			if (this->m_MemoryUsed + size + adjustment > this->m_MemorySize)
				return nullptr;
			
			this->m_MemoryUsed += size + adjustment;
			this->m_Allocations++;
			Hollow::Log::GetCoreLogger()->debug("LinearAllocator: allocated size {}, address {}", size, p);

			return p;
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