#ifndef __MOVE_SYSTEM_H__
#define __MOVE_SYSTEM_H__
#include "System.h"
#include "Engine/Common/Log.h"
#include "Engine/Events/MoveEvent.h"
#include "Engine/Entities/GameObject.h"
#include "Engine/Components/PositionComponent.h"
#include "Engine/Components/MoveComponent.h"
#include "Engine/Events/IEventListener.h"
#include "Engine/EventHandler.h"
#include <vector>

namespace Hollow {

	class MoveSystem : public System<MoveSystem>, IEventListener
	{
	public:
		MoveSystem() 
		{
			RegisterEventCallback<MoveEvent, MoveSystem>(&MoveSystem::Kek);
		}

		void Kek(const MoveEvent* const e)
		{
			Hollow::Log::GetCoreLogger()->critical("Bounce");
		}

		virtual void Update(float_t dt, std::vector<GameObject*>& gameObjects)
		{
			for (auto object : gameObjects) {
				PositionComponent * posComponent = object->GetComponent<PositionComponent>();
				MoveComponent * moveComponent = object->GetComponent<MoveComponent>();
				if (posComponent == nullptr || moveComponent == nullptr) continue;
				if (moveComponent->move) {
					posComponent->position.x -= 0.1f;
				} else {
					posComponent->position.x += 0.1f;
				}

				if (posComponent->position.x > 10.0f) {
					EventHandler::Get()->Send<MoveEvent>();
					moveComponent->move = true;
				}
				else if (posComponent->position.x < -10.0f) {
					EventHandler::Get()->Send<MoveEvent>();
					moveComponent->move = false;
				}
			}
		}
	};

}

#endif