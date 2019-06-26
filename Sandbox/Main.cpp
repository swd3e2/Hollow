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
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/ProjectSettings.h"

#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1440

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Core core(Hollow::RendererType::DirectX);

	Hollow::Window* window = Hollow::WindowManager::instance()->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, Hollow::WindowType::Borderless);
	Hollow::RenderApi* renderer = Hollow::RenderApiManager::instance()->initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	ProjectSettings settings;

	Hollow::Camera* camera = new Hollow::Camera(true);
	camera->SetProjectionValues(80.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 10000.0f);

	ForwardRenderSystem renderPass(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderPass.skyMap = new SkyMap();
	renderPass.m_Camera = camera;

	MoveSystem moveSystem(camera);

	core.systemManager.AddSystem(&renderPass);
	//core.systemManager.AddSystem(&moveSystem);

	GUISystem* gui = new GUISystem(window, renderer);
	gui->renderSystem = &renderPass;
	gui->mainCamera = camera;

	ProjectSettings::instance()->load("C:\\dev\\Hollow Engine\\Project1\\Project1.json");

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