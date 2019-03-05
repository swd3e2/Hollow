#pragma once
#include "Layer.h"
#include "Graphics/DirectX/D3DRenderer.h"

class ApplicationLayer : public Layer
{
private:
	D3DRenderer* renderer;
public:
	ApplicationLayer(D3DRenderer* renderer) :
		renderer(renderer)
	{}

	virtual void Update() override
	{
		renderer->Update();
	}

	virtual void PreUpdate() override
	{

	}

	virtual void PostUpdate() override
	{
	}
};