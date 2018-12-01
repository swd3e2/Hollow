#include "Engine/Core/ECS/Utils/Handle.h"
#include "Engine/Core/ECS/EntityManager.h"
#include "Engine/Core/ECS/Entity.h"
#include "Engine/Core/ECS/ComponentManager.h"
#include "Engine/Core/ECS/Component.h"

class PositionComponent : public ECS::Component<PositionComponent>
{
	float x;
	float y;
public:
	PositionComponent(float x, float y) :
		x(x), y(y)
	{}
};

class GameObject : public ECS::Entity<GameObject> {};
int main()
{
	ECS::ComponentManager cmanager;
	ECS::EntityManager manager(&cmanager);
	ECS::util::Handle64 kek = manager.CreateEntity<GameObject>();
	kek = manager.CreateEntity<GameObject>();
	kek = manager.CreateEntity<GameObject>();
	kek = manager.CreateEntity<GameObject>();
	kek = manager.CreateEntity<GameObject>();
	GameObject * object = (GameObject*)manager.GetEntity(kek);
	object->AddComponent<PositionComponent>(1.0f, 2.0f);
	size_t id = object->GetStaticEntityTypeID();
	system("pause");
	return 0;
}

