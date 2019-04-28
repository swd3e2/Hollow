#pragma once

#ifndef HW_POINT_LIGHT_COMPONENT_H
#define HW_POINT_LIGHT_COMPONENT_H

#include "Component.h"
#include "Hollow/Graphics/PointLight.h"

class PointLightComponent : public Component<PointLightComponent>
{
public:
	PointLight light;
public:
	PointLightComponent()
		: light()
	{}
};

#endif