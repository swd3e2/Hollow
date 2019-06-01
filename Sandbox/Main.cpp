#pragma once
#include <Hollow/Application.h>

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Application app;

	GameObject* entity = EntityManager::instance()->createEntity<GameObject>();

	entity->addComponent<TransformComponent, Vector3&&, Vector3&&, Vector3&&>
		(Vector3(0.0f, 33.0f, 22.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
	MeshImportData* mesh1 = app.meshImporter->import("Sandbox/Resources/Meshes/CharacterRunning.dae", false);

	entity->addComponent<RenderableComponent>(mesh1->meshData);
	entity->addComponent<AnimationComponent>(mesh1->animationData);

	app.Run();

	return 0;
}