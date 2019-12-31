#pragma once

#include <Hollow/Core.h>
#include "RenderSystem.h"
#include "CameraSystem.h"

class Game
{
	Hollow::Core core;
	Hollow::Window* window;
	Hollow::RenderApi* renderer;
	Hollow::Camera camera;

	Hollow::s_ptr<RenderSystem> renderSystem;
	Hollow::s_ptr<CameraSystem> cameraSystem;

	const int width = 1920;
	const int height = 1080;
public:
	Game()
	{
		window = Hollow::OGLWin32Window::startUp<Hollow::OGLWin32Window>(GetModuleHandle(NULL), width, height, Hollow::WindowType::Bordered);
		renderer = Hollow::RenderApi::startUp<Hollow::OGLRenderApi>(width, height);

		//camera.setProjectionValues(80.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 10000.0f);

		float zoom = 32.0f;
		float aspectRatio = 1920.0f / 1080.0f * zoom;
		camera.setOrthoValues(aspectRatio, -aspectRatio, zoom, -zoom, -1.0f, 1.0f);
		camera.setPosition(Hollow::Vector3(0.0f, 0.0f, -1.0f));

		renderSystem = std::make_shared<RenderSystem>(renderer, &camera);
		cameraSystem = std::make_shared<CameraSystem>(&camera);

		Hollow::SystemManager::instance()->addSystem(renderSystem.get());
		Hollow::SystemManager::instance()->addSystem(cameraSystem.get());
	}

	~Game()
	{
		Hollow::RenderApi::shutdown();
		Hollow::Window::shutdown();
	}

	void run()
	{
		while (!window->isClosed()) {
			core.preUpdate();

			window->processMessage();
			core.update();

			core.postUpdate();
		}
	}
};