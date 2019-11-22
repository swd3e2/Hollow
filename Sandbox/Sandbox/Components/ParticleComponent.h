#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Math/Vector3.h"
#include <list>

class ParticleComponent : public Hollow::Component<ParticleComponent>
{
public:
	struct Particle
	{
		Hollow::Vector3 position;
		Hollow::Vector3 velocity;
		double elapsed = 0.0;

		Particle(const Hollow::Vector3& position, const Hollow::Vector3& velocity) :
			position(position), velocity(velocity)
		{}
	};
public:
	float maxParticles;
	float lifetime;
	Hollow::s_ptr<Hollow::Texture> texture;
	std::list<Particle*> particles;
public:
	void emit(const Hollow::Vector3& position, const Hollow::Vector3& velocity)
	{
		if (particles.size() < maxParticles) {
			particles.push_back(new Particle(position, velocity));
		}
	}
};