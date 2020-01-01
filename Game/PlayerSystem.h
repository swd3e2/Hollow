#pragma once

#include <Hollow/ECS/System.h>
#include <Hollow/Input/InputManager.h>
#include "TransformComponent.h"
#include "GameObject.h"
#include <Hollow/ECS/EntityManager.h>

class PlayerSystem : public Hollow::System<PlayerSystem>
{
public:
	virtual void PreUpdate(double dt) override
	{}

	virtual void Update(double dt) override
	{
		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<TransformComponent>()) {
				TransformComponent* transform = entity.getComponent<TransformComponent>();

				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_A) == true) {
					transform->rotation.x += 0.01f;
				} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_D) == true) {
					transform->rotation.x -= 0.01f;
				}
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{}
};