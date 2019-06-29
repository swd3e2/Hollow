#include "Hollow/Core.h"
#include "Hollow/Memory/MemoryContainer.h"

struct Transform
{
	int x, y, z;
};
int main() 
{
	Hollow::Core;

	Hollow::MemoryContainer<Transform>;

	return 0;
}

