#pragma once
#include <vector>
#include "ECS/IEntity.h"
#include "Graphics/DirectX/D3DRenderable.h"
#include "Resources/TextureManager.h"
#include "Graphics/SimpleVertex.h"
#include "Resources/Mesh.h"
#include "Graphics/Transform.h"
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
	
	void CreateSceneObject(ID3D11Device * device, Mesh * mesh, Transform* transform);
	std::vector<IRenderable*>* GetSceneObjects();
};