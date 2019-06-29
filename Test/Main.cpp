#include "Hollow/Core.h"
#include "Hollow/Memory/MemoryContainer.h"

class Transform
{
public:
	int x, y, z;
	Transform(int x, int y, int z) :
		x(x), y(y), z(z)
	{}
};

int main() 
{
	Hollow::Core;

	Hollow::MemoryContainer<Transform> container;
	Transform* mem = new (container.allocate()) Transform(1,1,1);
	Transform* mem1 = new (container.allocate()) Transform(2,2,2);
	Transform* mem2 = new (container.allocate()) Transform(3,3,3);
	Transform* mem3 = new (container.allocate()) Transform(4,4,4);
	Transform* mem4 = new (container.allocate()) Transform(5,5,5);
	Transform* mem5 = new (container.allocate()) Transform(6,6,6);
	Transform* mem6 = new (container.allocate()) Transform(7,7,7);
	Transform* mem7 = new (container.allocate()) Transform(8,8,8);

	return 0;
}