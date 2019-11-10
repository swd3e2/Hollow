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
				const btVector3& prevVelocity = physics->body->getLinearVelocity();

				btVector3 moveVector(0.0f, 0.0f, 0.0f);
				moveVector.setY(prevVelocity.getY());

				physics->body->setLinearVelocity(btVector3(0, 0, 0.f));

				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_W)) {
					moveVector.setX(10.0f);
					transform->rotation.y = 0;
				} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_S)) {
					moveVector.setX(-10.0f);
					transform->rotation.y = Hollow::Math::PI;
				}
				
				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_A)) {
					moveVector.setZ(10.0f);

					if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_W)) {
						transform->rotation.y -= Hollow::Math::PI / 4;
					} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_S)) {
						transform->rotation.y += Hollow::Math::PI / 4;
					} else {
						transform->rotation.y = -Hollow::Math::PI / 2;
					}
				} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_D)) {
					moveVector.setZ(-10.0f);
					if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_W)) {
						transform->rotation.y += Hollow::Math::PI / 4;
					} else if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_S)) {
						transform->rotation.y -= Hollow::Math::PI / 4;
					} else {
						transform->rotation.y = Hollow::Math::PI / 2;
					}
				}

				if (Hollow::InputManager::GetKeyboardKeyIsPressed(Hollow::eKeyCodes::KEY_SPACE)) {
					moveVector.setY(10.0f);
				}
				physics->body->setLinearVelocity(moveVector);
				physics->body->activate(true);
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{}
};