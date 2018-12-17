#pragma once
#include "Hollow/Platform.h"

namespace Hollow { namespace Core { namespace Memory {

	// Allocators base class describing the allocator interface. 
	class HOLLOW_API BaseAllocator
	{
	protected:
		// Pointer to first address of allocated via malloc memory
		const void* m_MemoryFirstAddress;
		// Size of allocated memory
		size_t m_MemoryUsed;
		// Count of allocations made
		unsigned int m_Allocations;
		// Size of memory allocated
		const size_t m_MemorySize;
	public:
		BaseAllocator(void* pMem, unsigned int size) :
			m_MemoryUsed(0), m_MemorySize(size), m_MemoryFirstAddress(pMem), m_Allocations(0)
		{}

		virtual ~BaseAllocator() {}

		// Allocates memory by given size
		// @param size Size of memory needed to allocate
		// @param alignment Size of bytes needed
		// @return void* pointer to allocated memory
		virtual void* allocate(unsigned int size, unsigned int alignment) = 0;
				
		// Frees memory (except linear allocator)
		// @param pMmem pointer to memory that need to be freed
		virtual void free(void* pMmem) = 0;

		// Frees memory (except linear allocator).
		// Realization depends on kind of allocator
		virtual void clear() = 0;

		inline void* GetMemoryFirstAddress() const { return (void*)this->m_MemoryFirstAddress; }
		inline size_t GetMemoryUsed() const { return this->m_MemoryUsed; }
		inline size_t GetMemorySize() const { return this->m_MemorySize; }
	};

}}}