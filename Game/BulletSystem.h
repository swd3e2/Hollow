#pragma once

#include <Hollow/ECS/System.h>
#include <Hollow/Input/InputManager.h>
#include "TransformComponent.h"
#include "GameObject.h"
#include <Hollow/ECS/EntityManager.h>
#include <Hollow/Common/Log.h>
#include "Bullet.h"
#include "BulletComponent.h"
#include <Hollow/Core/DelayedTaskManager.h>
#include "GameObject.h"

class BulletSystem : public Hollow::System<BulletSystem>
{
public:
	virtual void PreUpdate(double dt) override {}

	virtual void Update(double dt) override
	{
		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<BulletComponent>() && entity.hasComponent<TransformComponent>()) {
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				BulletComponent* bullet = entity.getComponent<BulletComponent>();

				transform->position.x += bullet->velocity.x * dt / 1000.0;
				transform->position.y += bullet->velocity.y * dt / 1000.0;

				if (transform->position.x > 10.0f || transform->position.x < -10.0f 
					|| transform->position.y > 10.0f || transform->position.y < -10.0f) {
					Hollow::DelayedTaskManager::instance()->add([&](){
						Hollow::EntityManager::instance()->destroy(entity.getId());
					});
				}
			}
		}
	}

	virtual void PostUpdate(double dt) override {}
};