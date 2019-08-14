#pragma once
#include <Hollow/Math/Vector3.h>
#include <Hollow/ECS/Component.h>

enum class LightType {
	AMBIENT,
	DIFFUSE,
	POINT,
	SPOT
};

struct LightData
{
	Hollow::Vector3 position;	// 12
	float constant;				// 16
	Hollow::Vector3 direction;	// 28
	float linear;				// 32
	Hollow::Vector3 color;		// 44
	float quadratic;			// 48
	float cutoff;				// 52
	float distance;				// 56
	int type;					// 60
	int pad;					// 64
};

class LightComponent : public Hollow::Component<LightComponent>
{
public:
	LightData lightData;
};