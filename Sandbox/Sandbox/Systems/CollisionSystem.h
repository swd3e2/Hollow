#pragma once
#include "Hollow/ECS/Systems/System.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/PositionComponent.h"
#include "Sandbox/Components/CollisionComponent.h"
#include "Hollow/Engine.h"
#include "Hollow/Containers/vector.h"

class CollisionSystem : public Hollow::System<CollisionSystem>
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
		Hollow::Containers::Vector<IEntity*>* container = Hollow::Engine::Get()->GetEntityManager()->GetEntitiesList();
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

