#include "PoolAllocator.h"
#include <windows.h>
#include "array.h"
#include <cmath>
#include <iostream>

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

	Hollow::array<pos, 1024> array;
	for (int i = 0; i < 10000000; i++) {
		array.createObject<int>(5);
	}
	for (auto& it : array)
	{
		std::cout << it.x << std::endl;
	}


	system("pause");
	return 0;
}

