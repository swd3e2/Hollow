#pragma once
#include "Hollow/ECS/System.h"
#include "Hollow/ECS/Entities/GameObject.h"
#include "Hollow/ECS/Components/PositionComponent.h"
#include "Sandbox/Components/CollisionComponent.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/ECS/EntityManager.h"

using namespace Hollow;

class CollisionSystem : public System<CollisionSystem>
{
private:
	CollisionComponent * collisionComponent;
	PositionComponent * posComponent;
public:
	CollisionSystem()
	{
	}

	virtual void Update(float_t dt) override
	{
		Hollow::Containers::Vector<IEntity*>* container = EntityManager::instance()->GetEntitiesList();
		for (IEntity* object : *container) {
			collisionComponent = object->GetComponent<CollisionComponent>();
			posComponent = object->GetComponent<PositionComponent>();

			if (collisionComponent == nullptr || posComponent == nullptr) continue;
			
			if (posComponent->position.y > 0) {
				posComponent->position.y -= 0.01f;
			}
		}
		delete container;
	}
};
