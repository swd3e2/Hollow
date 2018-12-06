#include "Engine/Engine.h"
#define NOMINMAX
#include "windows.h"
#include "Engine/Memory/Allocators/LinearAllocator.h"
#include "Engine/Memory/Allocators/PoolAllocator.h"
#include "Engine/Memory/Allocators/StackAllocator.h"
#include "Engine/Memory/MemoryChunk.h"

struct MyComponent
{
	float x, y, z;
	char rotation;
};
void Test(MyComponent* component);
void LinearTest();

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Core::Memory::MemoryChunk<MyComponent> chunk;

	return 0;
}

void LinearTest()
{
	void* mem = malloc(67108864);
	Hollow::Core::Memory::LinearAllocator allocator(mem, 67108864);
	void* pMem;
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem);
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem);
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem);
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem);
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem, sizeof(MyComponent));
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem, sizeof(MyComponent));


	free(mem);
}

void PoolTest()
{
	void* mem = malloc(67108864);
	Hollow::Core::Memory::PoolAllocator allocator(mem, 67108864, sizeof(MyComponent), alignof(MyComponent));
	void* pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem);
	void* pMem1 = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem1);
	void* pMem2 = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem2);
	void* pMem3 = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	Test((MyComponent*)pMem3);
	void* pMem4 = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem4, sizeof(MyComponent));
	void* pMem5 = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem5, sizeof(MyComponent));


	allocator.free(pMem);
	allocator.free(pMem3);
	allocator.free(pMem5);

	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem, sizeof(MyComponent));

	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem, sizeof(MyComponent));
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem, sizeof(MyComponent));
	pMem = allocator.allocate(sizeof(MyComponent), alignof(MyComponent));
	ZeroMemory(pMem, sizeof(MyComponent));
}

void StackTest()
{
	void* mem = malloc(67108864);
	Hollow::Core::Memory::StackAllocator allocator(mem, 67108864);

	void* pMem;

	pMem = allocator.allocate(sizeof(MyComponent), alignof(unsigned char));
	ZeroMemory(pMem, sizeof(MyComponent));

	pMem = allocator.allocate(sizeof(MyComponent), alignof(unsigned char));
	ZeroMemory(pMem, sizeof(MyComponent));

	pMem = allocator.allocate(3 * sizeof(MyComponent), alignof(unsigned char));
	ZeroMemory(pMem, 3 * sizeof(MyComponent));

	pMem = allocator.allocate(2 * sizeof(MyComponent), alignof(unsigned char));
	ZeroMemory(pMem, 2 * sizeof(MyComponent));

	allocator.free(pMem);

	pMem = allocator.allocate(2 * sizeof(MyComponent), alignof(unsigned char));
	MyComponent* comp1 = (MyComponent*)pMem;
	comp1->x = 10000;
	comp1->y = 20000;
	comp1->z = 30000;
	MyComponent* comp2 = (MyComponent*)((void*)(reinterpret_cast<uintptr_t>(pMem) + sizeof(MyComponent)));
	comp2->x = 100000;
	comp2->y = 200000;
	comp2->z = 300000;
	ZeroMemory(pMem, 2 * sizeof(MyComponent));
}

void Test(MyComponent* component)
{
	component->x = 0;
	component->y = 1;
	component->z = 2;
	component->rotation = 'x';
}