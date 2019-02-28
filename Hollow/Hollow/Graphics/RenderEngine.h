#pragma once
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
#include <vector>
#include "IRenderable.h"
#include "DirectX/D3DRenderer.h"
#include "IRenderer.h"
#include "Camera.h"
#include "DirectX/D3DConstantBuffer.h"
#include "Hollow/Resources/Mesh.h"

class RenderEngine
{
protected:
	std::vector<IRenderable>			m_RenderableList;
	IRenderer*							renderApi;
public:
	RenderEngine()
	{
		renderApi = new D3DRenderer();
	}

	void Render(int renderableId)
	{

	}

	void CreateRendereable(Mesh* mesh)
	{
		this->renderApi->createRenderable(mesh);
	}

	IRenderable* GetRenderable(int renderableId)
	{
		return &m_RenderableList[renderableId];
	}

	

	void Update()
	{
	}

	void PreUpdate()
	{
	}

	void PostUpdate()
	{
	}

	bool processMessage()
	{
		return renderApi->processMessage();
	}
};
