#pragma once
#define NOMINMAX
#include <windows.h>
#include "../Engine/Core/Memory/Allocators/PoolAllocator.h"
#include "../Engine/Core/Memory/Allocators/StackAllocator.h"

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