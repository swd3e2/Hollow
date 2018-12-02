#ifndef _MANAGERS_TEST_H_
#define _MANAGERS_TEST_H_

#include "../Engine/ECS/Utils/Handle.h"
#include "../Engine/ECS/EntityManager.h"
#include "../Engine/ECS/Entity.h"
#include "../Engine/ECS/ComponentManager.h"
#include "../Engine/ECS/Component.h"

class PositionComponent : public ECS::Component<PositionComponent>
{
public:
	float x;
	float y;
	PositionComponent(float x, float y) :
		x(x), y(y)
	{}
};

class GameObject : public ECS::Entity<GameObject> {};

void ManagersTest();

#endif