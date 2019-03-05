#pragma once
#include "Hollow/ECS/Systems/System.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/PositionComponent.h"
#include "Sandbox/Components/MoveComponent.h"
#include <vector>
#include "Hollow/Containers/vector.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Hollow/Input/InputManager.h"
#include <DirectXMath.h>
#include "Hollow/ECS/EntityManager.h"

using namespace DirectX;

class MoveSystem : public Hollow::System<MoveSystem>
{
public:
	PositionComponent * posComponent;
	MoveComponent * moveComponent;
	SelectComponent* selectComponent;
public:
	MoveSystem() {}

	virtual void Update(float_t dt)
	{
		Hollow::Containers::Vector<Hollow::IEntity*>* container = Hollow::EntityManager::instance()->GetEntitiesList();

		for (Hollow::IEntity* object : *container) {
			posComponent = object->GetComponent<PositionComponent>();
			moveComponent = object->GetComponent<MoveComponent>();
			selectComponent = object->GetComponent<SelectComponent>();

			if (posComponent == nullptr || moveComponent == nullptr || selectComponent == nullptr) continue;

			if (InputManager::GetMouseButtonIsPressed(eMouseKeyCodes::MOUSE_LEFT)) {
				InputManager::calculate();
				moveComponent->elapsed = 0.0f;
				moveComponent->originalPosition = XMLoadFloat3(&posComponent->position);
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

				XMVECTOR passed = moveComponent->originalPosition - XMLoadFloat3(&posComponent->position);
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
		delete container;
	}
};
