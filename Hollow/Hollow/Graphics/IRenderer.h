#pragma once
#include "IRenderable.h"
#include <DirectXMath.h>
#include "Hollow/Resources/Mesh.h"
#include "Hollow/Platform.h"

class D3DRenderer;

class HOLLOW_API IRenderer
{
private:
	static IRenderer* _instance;
protected:
	IRenderer()
	{
		if (_instance == nullptr) {
			_instance = this;
		}
	}
public:
	static IRenderer* instance()
	{
		return _instance;
	}

	virtual bool processMessage() = 0;
	virtual size_t createRenderable(Mesh* mesh) = 0;
	virtual bool windowIsClosed() = 0;
	virtual void PreUpdateFrame() = 0;
	virtual void Update() = 0;
	virtual void PostUpdateFrame() = 0;
};