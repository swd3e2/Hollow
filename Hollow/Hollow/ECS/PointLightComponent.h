#pragma once

#ifndef HW_POINT_LIGHT_COMPONENT_H
#define HW_POINT_LIGHT_COMPONENT_H

#include "Component.h"
#include "Hollow/Graphics/PointLight.h"
#include <d3d11.h>

class PointLightComponent : public Component<PointLightComponent>
{
public:
	PointLight light;
public:
	PointLightComponent(ID3D11Device* device)
		: light(device)
	{}
};

#endif