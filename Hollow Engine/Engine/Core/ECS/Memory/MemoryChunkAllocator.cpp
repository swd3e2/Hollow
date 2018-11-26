#include "Allocators/PoolAllocator.h"
#include <list>

namespace ECS { namespace Memory {

	template<class OBJECT_CLASS, std::size_t MAX_CHUNK_OBJECTS>
	class MemoryChunkAllocator {
	private:
		static const std::size_t ALLOC_SIZE = (sizeof(OBJECT_TYPE) + alignof(OBJECT_TYPE)) * MAX_OBJECTS;

		class MemoryChunk {
		public: 
			Allocator::PoolAllocator* allocator;
			std::list<OBJECT_CLASS> objects;

			MemoryChunk(Allocator::PoolAllocator * in_allocator) : allocator(in_allocator) 
			{
				this->objects.clear();
			}
		};
	protected:
		std::list<MemoryChunk*> memoryChunks;
	public:
		MemoryChunkAllocator()
		{
			Allocator::PoolAllocator * allocator = new PoolAllocator(ALLOC_SIZE, Allocate());
		}
	};

}}