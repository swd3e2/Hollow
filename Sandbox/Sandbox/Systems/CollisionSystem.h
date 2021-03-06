#pragma once
#include <Hollow/ECS/System.h>
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/Containers/vector.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/CollisionComponent.h"

class CollisionSystem : public System<CollisionSystem>
{
private:
	CollisionComponent * collisionComponent;
	TransformComponent* posComponent;
public:
	virtual void Update(double dt) override
	{
		for (auto& entity : EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<CollisionComponent>() && entity.hasComponent<TransformComponent>()) {
				collisionComponent = entity.getComponent<CollisionComponent>();
				posComponent = entity.getComponent<TransformComponent>();

				if (posComponent->position.y > 0) {
					posComponent->position.y -= 0.01f;
				}
			}
		}
	}
};

