#pragma once

#include "Hollow/ECS/System.h"
#include "Hollow/ECS/EntityManager.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Components/ParticleComponent.h"
#include "Hollow/Common/Log.h"

class ParticleSystem : public Hollow::System<ParticleSystem>
{
public:

public:
	virtual void PreUpdate(double dt) override
	{
	}

	virtual void Update(double dt) override
	{
		double delta = dt / 1000.0;

		for (auto& entity : Hollow::EntityManager::instance()->container<GameObject>()) {
			if (entity.hasComponent<ParticleComponent>()) {
				ParticleComponent* particleComponent = entity.getComponent<ParticleComponent>();

				std::list<ParticleComponent::Particle*>::iterator it = particleComponent->particles.begin();

				while (it != particleComponent->particles.end()) {
					ParticleComponent::Particle* particle = *it;
					particle->position += particle->velocity * delta;
					HW_INFO("{} {} {}", particle->position.x, particle->position.y, particle->position.z);

					particle->position.y -= 0.1 * delta;
					particle->elapsed += delta;

					if (particle->elapsed > particleComponent->lifetime) {
						particleComponent->particles.erase(it++);
					} else {
						it++;
					}
				}
			}
		}
	}

	virtual void PostUpdate(double dt) override
	{
	}
};