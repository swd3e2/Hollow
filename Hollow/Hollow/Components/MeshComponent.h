#pragma once
#include "Hollow/ECS/Components/Component.h"
#include "Hollow/Graphics/BufferTemplate/IndexBuffer.h"
#include "Hollow/Graphics/BufferTemplate/VertexBuffer.h"
#include "Hollow/Graphics/SimpleVertex.h"

class MeshComponent : public Hollow::Component<MeshComponent>
{
public:
	MeshComponent(ID3D11Device * device, std::vector<SimpleVertex>* vdata, std::vector<unsigned int>* idata) :
		vBuffer(device, vdata->data(), vdata->size()), iBuffer(device, idata->data(), idata->size())
	{}

	Hollow::VertexBuffer<SimpleVertex> vBuffer;
	Hollow::IndexBuffer<unsigned int> iBuffer;
};
