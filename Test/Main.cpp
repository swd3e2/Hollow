#include "Hollow/Core.h"
#include "Hollow/Memory/MemoryContainer.h"

struct Transform
{
	int x, y, z;
};
int main() 
{
	Hollow::Core;

	Hollow::MemoryContainer<Transform> container;
	void* mem = container.allocate();
	void* mem1 = container.allocate();
	void* mem2 = container.allocate();
	void* mem3 = container.allocate();
	void* mem4 = container.allocate();
	void* mem5 = container.allocate();
	void* mem6 = container.allocate();



	return 0;
}

