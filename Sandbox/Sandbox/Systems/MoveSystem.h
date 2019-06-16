#pragma once
#include "Hollow/ECS/System.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Components/MoveComponent.h"
#include <vector>
#include "Hollow/Containers/vector.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Hollow/Input/InputManager.h"
#include <DirectXMath.h>
#include "Hollow/ECS/EntityManager.h"

using namespace DirectX;
using namespace Hollow;

class MoveSystem : public Hollow::System<MoveSystem>
{
public:
	TransformComponent* posComponent;
	MoveComponent * moveComponent;
	SelectComponent* selectComponent;
public:
	MoveSystem() {}

	virtual void Update(float_t dt)
	{
		for (auto& entity : EntityManager::instance()->getContainer<GameObject>()->entityList) {
			posComponent = entity.getComponent<TransformComponent>();
			moveComponent = entity.getComponent<MoveComponent>();
			selectComponent = entity.getComponent<SelectComponent>();

			if (posComponent == nullptr || moveComponent == nullptr || selectComponent == nullptr) continue;

			if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_LEFT)) {
				InputManager::calculate();
				moveComponent->elapsed = 0.0f;
				XMFLOAT3 tempFloat3(posComponent->position.x, posComponent->position.y, posComponent->position.z);
				moveComponent->originalPosition = XMLoadFloat3(&tempFloat3);
				DirectX::XMFLOAT3 temp(InputManager::pix, 0, InputManager::piz);
				moveComponent->destination = XMLoadFloat3(&temp);

				moveComponent->direction = moveComponent->originalPosition - moveComponent->destination;
				XMVECTOR length = XMVector3Length(moveComponent->direction);
				moveComponent->distance = 0.0f;
				XMStoreFloat(&moveComponent->distance, length);
				moveComponent->direction = XMVector4Normalize(moveComponent->direction);
				moveComponent->move = true;
			}

			if (moveComponent->move == true)
			{
				posComponent->position.x -= XMVectorGetX(moveComponent->direction) * moveComponent->speed * moveComponent->elapsed;
				posComponent->position.z -= XMVectorGetZ(moveComponent->direction) * moveComponent->speed * moveComponent->elapsed;

				XMFLOAT3 tempFloat3(posComponent->position.x, posComponent->position.y, posComponent->position.z);
				XMVECTOR passed = moveComponent->originalPosition - XMLoadFloat3(&tempFloat3);
				XMVECTOR length = XMVector3Length(passed);
				float distance = 0.0f;
				XMStoreFloat(&distance, length);

				if (distance >= moveComponent->distance)
				{
					posComponent->position = {InputManager::pix, 0, InputManager::piz};
					moveComponent->move = false;
				}
			}
			moveComponent->elapsed += dt;
		}
	}
};
