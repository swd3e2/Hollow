#pragma once
#include "IRenderable.h"
#include <DirectXMath.h>
#include "Hollow/Resources/Mesh.h"
#include "Hollow/Platform.h"
#include "Camera.h"
#include <vector>
#include "IRenderable.h"

class HOLLOW_API IRenderer
{
private:
	static IRenderer* _instance;
protected:
	Camera* m_Camera;
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

	inline void SetCamera(Camera* camera) { m_Camera = camera; }
	inline Camera* getCamera() { return m_Camera; }	
};