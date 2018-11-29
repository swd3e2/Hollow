#include "IAllocator.h"

namespace ECS { namespace Memory { namespace Allocators { 

	IAllocator::IAllocator(const std::size_t memSize, const void* mem): 
		m_MemorySize(memSize),
		m_MemoryFirstAddress(mem),
		m_MemoryUsed(0),
		m_MemoryAllocations(0)
	{}

	IAllocator::~IAllocator()
	{}

}}}