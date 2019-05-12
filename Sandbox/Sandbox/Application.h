#pragma once
#include "Hollow/Application.h"
#include "Hollow/Graphics/PointLight.h"
#include "Hollow/ECS/GameObject.h"
#include "Hollow/ECS/RenderableComponent.h"
#include "Hollow/ECS/TransformComponent.h"
#include "Hollow/ECS/PointLightComponent.h"
#include "Hollow/ECS/Light.h"
#include "Hollow/Importer/MeshImporter.h"

using namespace DirectX;

class MyApp : public Application
{
private:
public:
	void onStartUp()
	{
		GameObject* entity = EntityManager::instance()->createEntity<GameObject>();

		entity->addComponent<TransformComponent, const Vector3&, const Vector3&, const Vector3&>
			(Vector3( 0.0f, 0.0f, 0.0f ), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
		
		entity->addComponent<RenderableComponent>(MeshImporter::import("Sandbox/Resources/Meshes/CharacterRunning.dae", false));
		//entity->addComponent<RenderableComponent>(MeshImporter::import("Sandbox/Resources/Meshes/boblampclean.md5mesh", false));

		GameObject* entity2 = EntityManager::instance()->createEntity<GameObject>();

		entity2->addComponent<TransformComponent, const Vector3&, const Vector3&, const Vector3&>
			(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

		entity2->addComponent<RenderableComponent>(MeshImporter::import("Sandbox/Resources/Meshes/arrows.obj", false));
	}
};

