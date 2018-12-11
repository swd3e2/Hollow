#ifndef __MESH_COMPONENT__
#define __MESH_COMPONENT__
#pragma once
#include "Component.h"
#include "Engine/Graphics/BufferTemplate/IndexBuffer.h"
#include "Engine/Graphics/BufferTemplate/VertexBuffer.h"
#include "Engine/Graphics/SimpleVertex.h"

namespace Hollow {

	class MeshComponent : public Component<MeshComponent>
	{
	public:
		MeshComponent(ID3D11Device * device, std::vector<SimpleVertex>* vdata, std::vector<unsigned int>* idata) :
			vBuffer(device, vdata->data(), vdata->size()), iBuffer(device, idata->data(), idata->size())
		{}

		VertexBuffer<SimpleVertex> vBuffer;
		IndexBuffer<unsigned int> iBuffer;
	};

}

#endif