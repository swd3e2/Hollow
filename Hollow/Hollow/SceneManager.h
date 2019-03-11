#pragma once
#include <vector>
#include "ECS/IEntity.h"
#include "Graphics/DirectX/D3DRenderable.h"
#include "Graphics/TextureManager.h"
#include "Graphics/SimpleVertex.h"
#include "Resources/Mesh.h"
#include "Graphics/Transform.h"
#include "Hollow/Platform.h"
#include "Hollow/Graphics/DirectX/Icons/LightIcon.h"

class HOLLOW_API SceneManager
{
private:
	std::vector<IRenderable*> sceneObjects;
public:
	SceneManager() {}
	~SceneManager() {}

	void StartUp();
	void Shutdown();
	
	void CreateSceneObject(ID3D11Device * device, Mesh * mesh, Transform* transform);
	std::vector<IRenderable*>* GetSceneObjects();
};