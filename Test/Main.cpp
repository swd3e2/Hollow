#include "Hollow/Core.h"
#include "Hollow/Graphics/Renderer/Base/InputLayout.h"


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
	Hollow::InputLayout layout = {
		Hollow::InputDataType::Float,
		Hollow::InputDataType::Float,
		Hollow::InputDataType::Float,
		Hollow::InputDataType::Float
	};
	
	std::cin.get();

	return 0;
}