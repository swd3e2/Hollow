#pragma once

#include <Hollow/ECS/System.h>
#include <Hollow/Input/InputManager.h>
#include <Hollow/ECS/EntityManager.h>
#include "Sandbox/Components/PlayerComponent.h"
#include "Sandbox/Components/PhysicsComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"

class PlayerSystem : public Hollow::System<PlayerSystem>
{
public:
	virtual void PreUpdate(double dt) override
	{}

	virtual void Update(double dt) override
	{
		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<PlayerComponent>() && entity.hasComponent<PhysicsComponent>() && entity.hasComponent<TransformComponent>()) {
				TransformComponent* transform = entity.getComponent<TransformComponent>();
				PhysicsComponent* physics = entity.getComponent<PhysicsComponent>();
				
				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_W)) {
					physics->body->activate(true);
					physics->body->translate(btVector3(0.1f, 0, 0));
					transform->rotation.z = 0;
				} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_S)) {
					physics->body->activate(true);
					physics->body->translate(btVector3(-0.1f, 0, 0));
					transform->rotation.z = Hollow::Math::PI;
				}
				
				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_A)) {
					physics->body->activate(true);
					physics->body->translate(btVector3(0, 0, 0.1f));
					if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_W)) {
						transform->rotation.z += Hollow::Math::PI / 4;
					} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_S)) {
						transform->rotation.z -= Hollow::Math::PI / 4;
					} else {
						transform->rotation.z = Hollow::Math::PI / 2;
					}
				} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_D)) {
					physics->body->activate(true);
					physics->body->translate(btVector3(0, 0, -0.1f));
				}

				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_SPACE)) {
					physics->body->activate(true);
					physics->body->translate(btVector3(0, 0.15f, 0));
				}
				btTransform tr;
				physics->body->getMotionState()->getWorldTransform(tr);
				btVector3& pos = tr.getOrigin();
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{}
};