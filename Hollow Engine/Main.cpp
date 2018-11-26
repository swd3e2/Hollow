#define NOMINMAX
#include "windows.h"
#include "Engine/Core/ECS/Memory/Allocators/PoolAllocator.h"

int main()
{
	void * memory = malloc(1024);
	ECS::Memory::Allocator::PoolAllocator allocator{ 512, memory, 36, sizeof(std::size_t)};
	
	void * mem = allocator.allocate(36, sizeof(std::size_t));
	ZeroMemory(mem, 36);

	mem = allocator.allocate(36, sizeof(std::size_t));
	ZeroMemory(mem, 36);

	mem = allocator.allocate(36, sizeof(std::size_t));
	ZeroMemory(mem, 36);

	mem = allocator.allocate(36, sizeof(std::size_t));
	ZeroMemory(mem, 36);

	mem = allocator.allocate(36, sizeof(std::size_t));
	ZeroMemory(mem, 36);


	system("pause");
	return 0;
}