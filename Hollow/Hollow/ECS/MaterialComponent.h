#pragma once

#ifndef HW_MATERIAL_COMPONENT_H
#define HW_MATERIAL_COMPONENT_H

#include "Component.h"
#include "Hollow/Graphics/DirectX/D3DMaterial.h"

class MaterialComponent : public Component<MaterialComponent>
{
public:
	D3DMaterial material;
public:
	MaterialComponent() {}
};

#endif