#pragma once
#include "../../Platform.h"
#include "IAllocator.h"

namespace ECS { namespace Memory { namespace Allocator { 

	class StackAllocator : public IAllocator {
	private:
		struct AllocMetaInfo {
			u8 adjustment;
		};

	public:
		StackAllocator(std::size_t memSize, const void* mem);

		virtual ~StackAllocator();

		virtual void* allocate(std::size_t size, u8 alignment) override;
		virtual void free(void* p) override;
		virtual void clear() override;
	};

}}}