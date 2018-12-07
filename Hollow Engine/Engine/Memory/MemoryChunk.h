#pragma once

#ifndef __MEMORY_CHUNK__
#define __MEMORY_CHUNK__
#include "Allocators/PoolAllocator.h"
#include <list>

namespace Hollow { namespace Core { namespace Memory {

	template<class T>
	class MemoryChunk
	{
	public:
		PoolAllocator * allocator;
		unsigned int begin;
		unsigned int end;
		std::list<T*> objects;

		MemoryChunk(PoolAllocator * in_allocator) 
		{
			allocator = in_allocator;
			this->begin = (unsigned int)this->allocator->GetMemoryFirstAddress();
			this->end = this->begin + this->allocator->GetMemorySize();
			this->objects.clear();
		}
	};

}}}

#endif