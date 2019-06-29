#pragma once
#define OPENGL
#include <Hollow/Core.h>
#include "Sandbox/Components/TransformComponent.h"
#include "Sandbox/Entities/GameObject.h"
#include "Sandbox/Systems/ForwardRenderSystem.h"
#include "Sandbox/GUISystem.h"
#include "Sandbox/Systems/MoveSystem.h"
#include "Sandbox/Components/MoveComponent.h"
#include "Sandbox/Components/SelectComponent.h"
#include "Sandbox/Components/RenderableComponent.h"
#include "Sandbox/ProjectSettings.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
#include "Sandbox/Components/TransformComponent.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// App entrypoint
int main()
{
	Hollow::Core core(Hollow::RendererType::OpenGL);

	Hollow::Window* window = core.windowManager.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, Hollow::WindowType::Borderless);
	Hollow::RenderApi* renderer = core.renderApiManager.Initialize(SCREEN_WIDTH, SCREEN_HEIGHT);

	ProjectSettings::startUp();

	Hollow::Camera* camera = new Hollow::Camera(true);
	camera->SetProjectionValues(80.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 10000.0f);

	ForwardRenderSystem renderPass(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderPass.skyMap = new SkyMap();
	renderPass.m_Camera = camera;

	MoveSystem moveSystem(camera);

	SystemManager::instance()->AddSystem(&renderPass);
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