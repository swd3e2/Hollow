#pragma once
#include <vector>
#include "Graphics/DirectX/D3DRenderable.h"
#include "Resources/TextureManager.h"
#include "Graphics/Vertex.h"
#include "Resources/Mesh.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/DirectX/Icons/LightIcon.h"
#include "Hollow/Core/CModule.h"

class HOLLOW_API SceneManager : public CModule<SceneManager>
{
private:
	std::vector<IRenderable*> sceneObjects;
public:
	SceneManager() {}
	~SceneManager() {}

	void startUp();
	void shutdown();
	
	std::vector<IRenderable*>* GetSceneObjects();
};