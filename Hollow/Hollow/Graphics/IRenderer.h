#pragma once
#include "IRenderable.h"
#include <DirectXMath.h>
#include "Hollow/Resources/Mesh.h"

class D3DRenderer;

class IRenderer
{
private:
	static IRenderer* _instance;
protected:
	IRenderer()
	{
		
	}
public:
	

	static IRenderer* instance()
	{
		if (_instance = nullptr) {
			_instance = new D3DRenderer();
		}
		return _instance;
	}

	virtual bool processMessage() = 0;
	virtual void Draw(IRenderable& renderable) = 0;
	virtual size_t createRenderable(Mesh* mesh) = 0;
	virtual void PreUpdateFram() = 0;
	virtual void Update() = 0;
	virtual void PostUpdateFrame() = 0;
};