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
		
		entity->addComponent<RenderableComponent>(MeshImporter::import("gwen.obj"));
	}
};