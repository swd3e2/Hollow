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