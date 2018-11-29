#include "Engine/Core/ECS/Utils/FamilyTypeID.h"
#include <iostream>
#include "Engine/Core/Memory/Allocators/PoolAllocator.h"
#include "Engine/Core/Memory/Allocators/StackAllocator.h"
#define NOMINMAX
#include <windows.h>
#include "Engine/Core/ECS/Utils/Handle.h"
#include "Engine/Core/ECS/EntityManager.h"
#include "Engine/Core/ECS/Entity.h"

class GameObject : public ECS::Entity<GameObject> {};
int main()
{
	ECS::EntityManager manager;
	size_t kek = manager.CreateEntity<GameObject>();
	size_t kek = manager.CreateEntity<GameObject>();
	size_t kek = manager.CreateEntity<GameObject>();
	size_t kek = manager.CreateEntity<GameObject>();
	size_t kek = manager.CreateEntity<GameObject>();

	system("pause");
	return 0;
}

void AllocatorsTest()
{
	void* stackmem = malloc(512);
	void* currMem;
	ECS::Memory::Allocators::StackAllocator stackAllocator{ 512, stackmem };
	currMem = stackAllocator.allocate(36, 1);
	ZeroMemory(currMem, 36);
	currMem = stackAllocator.allocate(36, 1);
	ZeroMemory(currMem, 36);
	currMem = stackAllocator.allocate(36, 1);
	ZeroMemory(currMem, 36);

	void* poolmem = malloc(512);
	ECS::Memory::Allocators::PoolAllocator poolAllocator{ 512, poolmem, 36, 1 };
	currMem = poolAllocator.allocate(36, 1);
	ZeroMemory(currMem, 36);
	currMem = poolAllocator.allocate(36, 1);
	ZeroMemory(currMem, 36);
	currMem = poolAllocator.allocate(36, 1);
	ZeroMemory(currMem, 36);
}