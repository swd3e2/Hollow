#pragma once
#include "../../Platform.h"

namespace ECS { namespace Memory { namespace Allocators {

	/* Base allocator class. */
	class IAllocator
	{
	protected:
		const std::size_t		m_MemorySize;
		const void*				m_MemoryFirstAddress;
		std::size_t				m_MemoryUsed;
		u64						m_MemoryAllocations;
	public:
		IAllocator(const std::size_t memSize, const void* mem);
		virtual ~IAllocator();

		virtual void* allocate(std::size_t size, u8 alignment) = 0;
		virtual void free(void* p) = 0;
		virtual void clear() = 0;

		inline std::size_t GetMemorySize() const
		{
			return this->m_MemorySize;
		}

		inline const void* GetMemoryAddress0() const
		{
			return this->m_MemoryFirstAddress;
		}

		inline std::size_t GetUsedMemory() const
		{
			return this->m_MemoryUsed;
		}

		inline u64 GetAllocationCount() const
		{
			return this->m_MemoryAllocations;
		}
	};

}}}