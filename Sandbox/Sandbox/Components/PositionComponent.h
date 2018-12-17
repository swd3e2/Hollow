#pragma once
#include "Hollow/ECS/Components/Component.h"
#include "directXMath.h"

class PositionComponent : public Hollow::Component<PositionComponent>
{
public:
	PositionComponent(float x, float y, float z, float h) :
		position(x, y, z, h)
	{}
	DirectX::XMFLOAT4 position;
};
