#ifndef __STACK_ALLOCATOR_H__
#define __STACK_ALLOCATOR_H__
#include "BaseAllocator.h"
#include "PointerMathUtils.h"
#include "Platform.h"

namespace Hollow { namespace Core { namespace Memory {

	// Class StackAllocator
	class HOLLOW_API StackAllocator : public BaseAllocator
	{
		struct AllocatorHeader {
			unsigned char alignment;
		};

	public:
		StackAllocator(void* pMem, unsigned int size) :
			BaseAllocator(pMem, size)
		{}

		virtual ~StackAllocator()
		{
			this->clear();
		}

		virtual void* allocate(unsigned int size, unsigned int alignment)
		{
			void* p = (void*)(reinterpret_cast<uintptr_t>(this->m_MemoryFirstAddress) + this->m_MemoryUsed);

			unsigned char adjustment = GetAdjustmentWithHeader(p, alignment, sizeof(AllocatorHeader));

			if (this->m_MemoryUsed + size + adjustment > this->m_MemorySize)
			{
				return nullptr;
			}

			((AllocatorHeader*)p)->alignment = adjustment;

			p = (void*)(reinterpret_cast<uintptr_t>(p) + sizeof(AllocatorHeader));

			this->m_Allocations++;
			this->m_MemoryUsed += size + adjustment;

			return p;
		}

		virtual void free(void* pMem)
		{
			unsigned char adjustment = ((AllocatorHeader*)(reinterpret_cast<uintptr_t>(pMem) - sizeof(AllocatorHeader)))->alignment;
			
			this->m_Allocations--;
			this->m_MemoryUsed -= reinterpret_cast<uintptr_t>(this->m_MemoryFirstAddress) + this->m_MemoryUsed - reinterpret_cast<uintptr_t>(pMem) + adjustment;
		}

		virtual void clear()
		{
			this->m_Allocations = 0;
			this->m_MemoryUsed = 0;
		}

	};

}}}


#endif