#include "Hollow/Core.h"
#include "Hollow/Memory/MemoryContainer.h"
#include "Hollow/Common/Logger.h"
#include <iostream>
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
	Hollow::Logger::startUp();

	for (int i = 0; i < 1000; i++) {
		Hollow::Logger::instance()->log("{}: {} {}", "some", 1.0f, 2);
	}

	std::cin.get();

	return 0;
}