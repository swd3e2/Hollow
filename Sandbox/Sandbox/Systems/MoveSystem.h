#pragma once
#include "Hollow/ECS/Systems/System.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Events/MoveEvent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/PositionComponent.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Hollow/Events/IEventListener.h"
#include "Hollow/ECS/EventHandler.h"
#include <vector>

class MoveSystem : public Hollow::System<MoveSystem>, Hollow::IEventListener
{
public:
	MoveSystem() 
	{
		this->RegisterEventCallback<MoveEvent, MoveSystem>(&MoveSystem::Kek);
	}

	void Kek(const MoveEvent* const e)
	{
		//Hollow::Log::GetCoreLogger()->critical("Bounce");
	}

	virtual void Update(float_t dt, std::vector<GameObject*>& gameObjects)
	{
		for (auto object : gameObjects) {
			PositionComponent * posComponent = object->GetComponent<PositionComponent>();
			MoveComponent * moveComponent = object->GetComponent<MoveComponent>();
			if (posComponent == nullptr || moveComponent == nullptr) continue;
			if (moveComponent->move) {
				posComponent->position.x -= 0.5f;
			} else {
				posComponent->position.x += 0.5f;
			}

			if (posComponent->position.x > 350.0f) {
				Hollow::EventHandler::Get()->Send<MoveEvent>();
				moveComponent->move = true;
			}
			else if (posComponent->position.x < -350.0f) {
				Hollow::EventHandler::Get()->Send<MoveEvent>();
				moveComponent->move = false;
			}
		}
	}
};

