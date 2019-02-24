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
#include "Hollow/Resources/Sound.h"
#include "Hollow/Containers/vector.h"

class MoveSystem : public Hollow::System<MoveSystem>, Hollow::IEventListener
{
public:
	Hollow::Sound* sound;
	PositionComponent * posComponent;
	MoveComponent * moveComponent;
public:
	MoveSystem() 
	{
	}

	virtual void Update(float_t dt)
	{
		Hollow::Containers::Vector<Hollow::IEntity*>* container = Hollow::Engine::Get()->GetEntityManager()->GetEntitiesList();

		for (Hollow::IEntity* object : *container) {
			posComponent = object->GetComponent<PositionComponent>();
			moveComponent = object->GetComponent<MoveComponent>();
			if (posComponent == nullptr || moveComponent == nullptr) continue;
		}
		delete container;
	}
};
