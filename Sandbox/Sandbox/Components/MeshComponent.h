#pragma once
#include "Hollow/ECS/Components/Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class MeshComponent : public Hollow::Component<MeshComponent>
{
public:
	MeshComponent(size_t renderableId) : renderableId(renderableId) {}
	size_t renderableId;
};
