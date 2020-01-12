#pragma once

#include <Hollow/Core.h>
#include "RenderSystem.h"
#include "CameraSystem.h"
#include "PlayerSystem.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "PlayerComponent.h"
#include <Hollow/Common/Log.h>
#include "TextureManager.h"
#include "BulletSystem.h"
#include <Hollow/Math/Matrix4.h>

class Game
{
	Hollow::Core core;
	Hollow::Window* window;
	Hollow::RenderApi* renderer;
	Hollow::Camera camera;

	Hollow::s_ptr<RenderSystem> renderSystem;
	Hollow::s_ptr<CameraSystem> cameraSystem;
	Hollow::s_ptr<PlayerSystem> playerSystem;
	Hollow::s_ptr<BulletSystem> bulletSystem;

	const int width = 1920;
	const int height = 1080;
public:
	Game()
	{
		window = Hollow::OGLWin32Window::startUp<Hollow::OGLWin32Window>(GetModuleHandle(NULL), width, height, Hollow::WindowType::Borderless);
		renderer = Hollow::RenderApi::startUp<Hollow::OGLRenderApi>(width, height);

		TextureManager::startUp();

		//camera.setProjectionValues(80.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 10000.0f);

		float zoom = 32.0f;
		float aspectRatio = 1920.0f / 1080.0f * zoom;
		camera.setOrthoValues(aspectRatio, -aspectRatio, zoom, -zoom, -1.0f, 1.0f);
		camera.setPosition(Hollow::Vector3(0.0f, 0.0f, -1.0f));

		renderSystem = std::make_shared<RenderSystem>(renderer, &camera);
		cameraSystem = std::make_shared<CameraSystem>(&camera);
		playerSystem = std::make_shared<PlayerSystem>(&camera);
		bulletSystem = std::make_shared<BulletSystem>();

		Hollow::SystemManager::instance()->addSystem(renderSystem.get());
		Hollow::SystemManager::instance()->addSystem(cameraSystem.get());
		Hollow::SystemManager::instance()->addSystem(playerSystem.get());
		Hollow::SystemManager::instance()->addSystem(bulletSystem.get());

		GameObject* gameObject = Hollow::EntityManager::instance()->create<GameObject>();
		gameObject->addComponent<PlayerComponent>();
		TransformComponent* transformComponent = gameObject->addComponent<TransformComponent>();
		{
			RenderComponent* renderComponent = gameObject->addComponent<RenderComponent>();

			/*Vertex vertices[] = {
				Vertex({  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f }),
				Vertex({  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }),
				Vertex({ -1.0f,  1.0f, 0.0f }, { 0.0f, 0.0f }),
				Vertex({ -1.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }),
			};
			renderComponent->vertexBuffer = Hollow::VertexBuffer::create({ vertices, 4, sizeof(Vertex) });

			unsigned int indices[] = { 0, 1, 2, 2, 1, 3 };
			renderComponent->indexBuffer = Hollow::IndexBuffer::create({ indices, 6, Hollow::IndexFormat::IFT_UINT });*/

			//renderComponent->texture = TextureManager::instance()->create2DTextureFromFile("E:/Hollow/Game/testt.png");
			
			Vertex vertices[] = {
				Vertex({  0.0f,  3.5f, 0.0f }, { 1.0f, 0.0f }),
				Vertex({  1.0f,  0.0f, 0.0f }, { 1.0f, 1.0f }),
				Vertex({ -1.0f,  0.0f, 0.0f }, { 0.0f, 0.0f })
			};
			renderComponent->vertexBuffer = Hollow::VertexBuffer::create({ vertices, 3, sizeof(Vertex) });

			unsigned int indices[] = { 0, 1, 2 };
			renderComponent->indexBuffer = Hollow::IndexBuffer::create({ indices, 3, Hollow::IndexFormat::IFT_UINT });
		}
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