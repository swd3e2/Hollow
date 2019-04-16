#pragma once
#include "Component.h"
#include "Hollow/Resources/Mesh.h"

class MeshComponent : public Component<MeshComponent>
{
public:
	Mesh* mesh;
};