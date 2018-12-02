#include "ManagersTest.h"

void ManagersTest()
{
	ECS::ComponentManager cmanager;
	ECS::EntityManager manager(&cmanager);
	ECS::util::Handle64 kek = manager.CreateEntity<GameObject>();
	GameObject * object = (GameObject*)manager.GetEntity(kek);
	object->AddComponent<PositionComponent>(1.0f, 2.0f);
	GameObject * object2 = (GameObject*)manager.GetEntity(kek);
	object2->AddComponent<PositionComponent>(1.0f, 2.0f);
	float x, y;
	for (auto it = cmanager.begin<PositionComponent>(); cmanager.end<PositionComponent>() != it; ++it)
	{
		x = it->x;
		y = it->y;
	}

	size_t id = object->GetStaticEntityTypeID();
}