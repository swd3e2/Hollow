#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Math/Vector2.h>

class BulletComponent : public Hollow::Component<BulletComponent>
{
public:
	Hollow::Vector2 velocity;
};