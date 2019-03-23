#include "PoolAllocator.h"
#include <windows.h>
#include "array.h"

class pos
{
public:
	int x;
	int y;
	int z;
	int w;
	long long kek1;

	pos(int x) : x(x) {}
	pos() {}
};

int main() 
{
	PoolAllocator alloc(10, sizeof(pos), alignof(pos));
	size_t kek = alignof(pos);
	
	void* mem = alloc.allocate();
	void* mem1 = alloc.allocate();
	void* mem2 = alloc.allocate();

	memset(mem, 1, sizeof(pos));
	memset(mem1, 1, sizeof(pos));
	memset(mem2, 1, sizeof(pos));

	alloc.deallocate(mem2);
	alloc.deallocate(mem1);

	void* mem3 = alloc.allocate();
	void* mem4 = alloc.allocate();
	void* mem5 = alloc.allocate();

	memset(mem1, 0, sizeof(pos));
	memset(mem2, 0, sizeof(pos));

	Hollow::array<pos> array;
	pos* posComponent = array.createObject();
	posComponent->kek1 = 112312312;
	posComponent->x = 46345;
	posComponent->y = 12312;
	posComponent->z = 235235;
	posComponent->w = 586354;

	pos* posComponent1 = array.createObject();
	pos* posComponent2 = array.createObject<int>(5);

	pos* posComponent11 = array[0];
	posComponent11->x = 100000;
	posComponent11->y = 2000;

	system("pause");
	return 0;
}

