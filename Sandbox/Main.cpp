#pragma once
#include <Hollow/Core.h>
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Systems/RenderSystem.h"
#include "Sandbox/GUI/GUISystem.h"
#include "Sandbox/Systems/MoveSystem.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/ProjectSettings.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/Terrain.h"
#include "Sandbox/Components/TerrainData.h"
#include "Hollow/Graphics/Base/InputLayout.h"
#include "Sandbox/Entities/Light.h"
#include "Sandbox/Components/LightComponent.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Core core;

	Hollow::Window* window = WindowManager::create(Hollow::RendererType::OpenGL, SCREEN_WIDTH, SCREEN_HEIGHT, Hollow::WindowType::Bordered);
	Hollow::RenderApi* renderer = RenderApiManager::create(Hollow::RendererType::OpenGL, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	ProjectSettings::startUp<ProjectSettings>(Hollow::RendererType::OpenGL);

	Hollow::Camera* camera = new Hollow::Camera(true);
	camera->setProjectionValues(80.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100000.0f);

	RenderSystem renderPass(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderPass.skyMap = new SkyMap();
	renderPass.m_Camera = camera;

	MoveSystem moveSystem(camera);

	SystemManager::instance()->addSystem(&renderPass);
	SystemManager::instance()->addSystem(&moveSystem);

	GUISystem* gui = new GUISystem(window, renderer);
	gui->rendererTab.renderSystem = &renderPass;

	ProjectSettings::instance()->load("C:\\dev\\Hollow Engine\\Project1\\Project1.json");

	/*Light* light = Hollow::EntityManager::instance()->create<Light>();
	light->addComponent<TransformComponent>();
	light->addComponent<LightComponent>();*/

	while (!window->isClosed()) {
		core.preUpdate();

		window->processMessage();
		camera->update(core.dt);
		core.update();

		gui->update(core.dt);

		core.postUpdate();
	}
	
	return 0;
}