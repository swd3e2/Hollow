#ifndef __POSITION_COMPONENT__
#define __POSITION_COMPONENT__
#pragma once

#include "Component.h"
#include "directXMath.h"

namespace Hollow {

	class PositionComponent : public Component<PositionComponent>
	{
	public:
		PositionComponent(float x, float y, float z, float h) :
			position(x, y, z, h)
		{}
		DirectX::XMFLOAT4 position;
	};

}

#endif