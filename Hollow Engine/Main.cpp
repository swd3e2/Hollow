#include "Engine/Engine.h"
#define NOMINMAX
#include "windows.h"
#include "Engine/Memory/Allocators/LinearAllocator.h"
#include "Engine/Memory/Allocators/PoolAllocator.h"


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

void Test(MyComponent* component)
{
	component->x = 0;
	component->y = 1;
	component->z = 2;
	component->rotation = 'x';
}