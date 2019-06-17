#pragma once
#define D3D11
#include <Hollow/Core.h>
#include <Hollow/Importer/gltf/GLTFImporter.h>
#include "Sandbox/Components/GLTFRenderable.h"
#include "Sandbox/Components/GLTFAnimation.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Systems/ForwardRenderSystem.h"
#include "Sandbox/Systems/AnimationSystem.h"
#include <Hollow/Graphics/RenderApiManager.h>
#include "Sandbox/GUISystem.h"
#include "Sandbox/Systems/MoveSystem.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Sandbox/Components/SelectComponent.h"

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Core core;

	Hollow::Window* window = Hollow::WindowManager::instance()->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);
	Hollow::RenderApi* renderer = Hollow::RenderApiManager::instance()->initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	Hollow::Camera* camera = new Hollow::Camera(true);
	camera->SetProjectionValues(90.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 10000.0f);

	ForwardRenderSystem renderPass(renderer);
	renderPass.skyMap = new SkyMap();
	renderPass.m_Camera = camera;

	MoveSystem moveSystem(camera);

	Hollow::GLTFImporter importer;
	Hollow::GLTF::GLTFModel* model = importer.import("Sandbox/Resources/Meshes/scene.gltf");

	GameObject* entity = Hollow::EntityManager::instance()->createEntity<GameObject>();
	entity->addComponent<GLTFAnimation, Hollow::GLTF::GLTFModel*&>(model);
	entity->addComponent<GLTFRenderable, Hollow::GLTF::GLTFModel*&>(model);

	entity->addComponent<TransformComponent, Hollow::Vector3&&, Hollow::Vector3&&, Hollow::Vector3&&>
		(Hollow::Vector3(0.0f, 0.0f, 0.0f), Hollow::Vector3(1.0f, 1.0f, 1.0f), Hollow::Vector3(-1.5f, 0.0f, 0.0f));
	
	entity->addComponent<MoveComponent>();


	core.systemManager.AddSystem(&renderPass);
	core.systemManager.AddSystem(&moveSystem);

	GUISystem* gui = new GUISystem(window, renderer);
	gui->renderSystem = &renderPass;

	while (!window->isClosed()) {
		core.PreUpdate();

		window->ProcessMessage();
		camera->Update(core.dt);
		core.Update();
		gui->update(core.dt);

		core.PostUpdate();
	}

	return 0;
}