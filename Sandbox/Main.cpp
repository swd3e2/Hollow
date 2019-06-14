#pragma once
#include <Hollow/Application.h>
#include <Hollow/Importer/gltf/GLTFImporter.h>
#include "Hollow/ECS/GLTFRenderable.h"
#include <Hollow/ECS/GLTFAnimationComponent.h>

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Application app;

	GLTFImporter importer;
	Hollow::GLTFModel* model = importer.import("Sandbox/Resources/Meshes/scene.gltf");

	GameObject* entity = EntityManager::instance()->createEntity<GameObject>();

	entity->addComponent<GLTFAnimationComponent, Hollow::GLTFModel*&>(model);
	entity->addComponent<GLTFRenderable, Hollow::GLTFModel*&>(model);

	entity->addComponent<TransformComponent, Vector3&&, Vector3&&, Vector3&&>
		(Vector3(0.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));


	app.Run();

	return 0;
}