#pragma once
#include "Hollow/ECS/Components/Component.h"
#include "Hollow/Graphics/BufferTemplate/IndexBuffer.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"
#include "Hollow/Graphics/SimpleVertex.h"
#include "Hollow/Resources/ResourceManager.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Resources/Mesh.h"

class MeshComponent : public Hollow::Component<MeshComponent>
{
public:
	MeshComponent(Hollow::Mesh* mesh) : mesh(mesh) {}
	
	~MeshComponent()
	{
		if (mesh != nullptr)
		{
			delete mesh;
		}
	}

	Hollow::Mesh* mesh;
};
