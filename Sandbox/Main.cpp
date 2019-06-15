#pragma once
#include <Hollow/Application.h>
#include <Hollow/Importer/gltf/GLTFImporter.h>
#include "Hollow/ECS/GLTFRenderable.h"
#include "Hollow/ECS/GLTFAnimation.h"


// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	GLTFImporter importer;
	Hollow::GLTFModel* model = importer.import("Sandbox/Resources/Meshes/scene.gltf");

	Application app;

	GameObject* entity = EntityManager::instance()->createEntity<GameObject>();
	entity->addComponent<GLTFAnimation, Hollow::GLTFModel*&>(model);
	entity->addComponent<GLTFRenderable, Hollow::GLTFModel*&>(model);

	entity->addComponent<TransformComponent, Vector3&&, Vector3&&, Vector3&&>
		(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.8f, 0.8f, 0.8f), Vector3(0.0f, 0.0f, 0.0f));

	//MeshImportData* mesh1 = app.meshImporter->import("Sandbox/Resources/Meshes/scene.gltf", false);

	//entity->addComponent<RenderableComponent>(mesh1->meshData);
	////entity->addComponent<AnimationComponent>(mesh1->animationData);

	app.Run();

	return 0;
}