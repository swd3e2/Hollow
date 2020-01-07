#pragma once

#include <Hollow/ECS/Component.h>
#include <Hollow/Math/Vector3.h>

class TransformComponent : public Hollow::Component<TransformComponent>
{
public:
	Hollow::Vector3 rotation = Hollow::Vector3(0.0f, 0.0f, 0.0f);
	Hollow::Vector3 position = Hollow::Vector3(0.0f, 0.0f, 0.0f);
	Hollow::Vector3 screenPos = Hollow::Vector3(0.0f, 0.0f, 0.0f);
};