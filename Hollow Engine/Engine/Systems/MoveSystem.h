#ifndef __MOVE_SYSTEM_H__
#define __MOVE_SYSTEM_H__
#include "System.h"
#include "Engine/Entities/GameObject.h"
#include "Engine/Components/PositionComponent.h"
#include "Engine/Components/MoveComponent.h"
#include <vector>

namespace Hollow {

	class MoveSystem : public System<MoveSystem>
	{
	public:
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

				if (posComponent->position.x > 50.0f) {
					moveComponent->move = true;
				}
				else if (posComponent->position.x < -50.0f) {
					moveComponent->move = false;
				}
			}
		}
	};

}

#endif