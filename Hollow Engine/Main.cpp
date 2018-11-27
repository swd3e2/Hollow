
#include "Engine/Core/ECS/Utils/FamilyTypeID.h"
#include <iostream>

class IEntity {};
class Entity{};
int main()
{
	std::cout << ECS::util::Internal::FamilyTypeID<IEntity>::Get<Entity>() << std::endl;

	system("pause");
	return 0;
}