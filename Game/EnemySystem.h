#pragma once

#include <Hollow/ECS/System.h>
#include "Enemy.h"
#include "TransformComponent.h"
#include <Hollow/Math/Vector2.h>
#include <Hollow/ECS/EntityManager.h>
#include "PlayerComponent.h"
#include "GameObject.h"

class EnemySystem : public Hollow::System<EnemySystem>
{
public:
	GameObject* player;
public:
	EnemySystem(GameObject* player) :
		player(player)
	{}

	virtual void PreUpdate(double dt) override
	{}

	virtual void Update(double dt) override
	{
		for (auto& enemy : Hollow::EntityManager::instance()->container<Enemy>()) {
			if (enemy.hasComponent<TransformComponent>()) {
				TransformComponent* enemyTransform = enemy.getComponent<TransformComponent>();
				TransformComponent* transform = player->getComponent<TransformComponent>();

				Hollow::Vector2 temp(transform->position.x - enemyTransform->position.x, transform->position.y - enemyTransform->position.y);
				temp = Hollow::Vector2::normalize(temp);

				enemyTransform->position.x += temp.x * 0.001f * dt;
				enemyTransform->position.y += temp.y * 0.001f * dt;
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{}
};