#pragma once

#include "Hollow/ECS/Component.h"
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Math/Vector3.h"
#include <list>
#include "Hollow/Graphics/Texture.h"
#include "Hollow/Math/Vector2.h"

class ParticleComponent : public Hollow::Component<ParticleComponent>
{
public:
	struct Particle
	{
		Hollow::Vector3 position;
		Hollow::Vector3 scale;
		Hollow::Vector3 velocity;
		double elapsed = 0.0;
		double offsetUpdateTime = 0.0;
		int currentOffset = 0;

		Particle(const Hollow::Vector3& position, const Hollow::Vector3& velocity, const Hollow::Vector3& scale) :
			position(position), velocity(velocity), scale(scale)
		{}
	};
public:
	int maxOffsets;
	int currentOffset = 0;
	int maxParticles;
	Hollow::Vector2 texCoords;
	float lifetime;
	std::list<Particle*> particles;
	Hollow::s_ptr<Hollow::Texture> texture;
	float timeToUpdate = 1.0f;
public:
	void emit(const Hollow::Vector3& position, const Hollow::Vector3& velocity, const Hollow::Vector3& scale)
	{
		if (particles.size() < maxParticles) {
			particles.push_back(new Particle(position, velocity, scale));
		}
	}
};