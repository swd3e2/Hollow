#pragma once
#include "Hollow/ECS/Components/Component.h"
#include "directXMath.h"

class PositionComponent : public Hollow::Component<PositionComponent>
{
public:
	PositionComponent(DirectX::XMFLOAT3&& position, DirectX::XMFLOAT3&& scale, DirectX::XMFLOAT3&& rotation) :
		position(position), scale(scale), rotation(rotation)
	{}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT3 rotation;
};
