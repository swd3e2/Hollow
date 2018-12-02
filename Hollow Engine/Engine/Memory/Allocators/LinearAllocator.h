#ifndef __LINEAR_ALLOC_H__
#define __LINEAR_ALLOC_H__

#include "IAllocator.h"
#include <assert.h>

namespace ECS { namespace Memory { namespace Allocators {

	class LinearAllocator : public IAllocator
	{
	public:

		LinearAllocator(size_t memSize, const void* mem);

		virtual ~LinearAllocator();

		virtual void* allocate(size_t size, u8 alignment) override;
		virtual void free(void* p) override;
		virtual void clear() override;

	}; 

}}}

#endif