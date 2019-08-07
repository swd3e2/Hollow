#pragma once
#include <Hollow/Math/Vector3.h>
#include <Hollow/ECS/Component.h>

enum LightType {
	AMBIENT,
	DIFFUSE,
	POINT,
	SPOT
};

struct LightData
{
	float position; // 4
	float constant; // 8
	float linear; // 12
	float quadratic; // 16
	float cutoff; // 20
	float direction; // 24
	Hollow::Vector3 color; // 36

	int type; // 40
};

class LightComponent : Hollow::Component<LightComponent>
{
public:
	LightData lightData;
};