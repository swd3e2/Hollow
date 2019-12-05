#pragma once

#include <Hollow/Core.h>
#include <Hollow/ECS/EntityManager.h>
#include <Hollow/ECS/ComponentManager.h>
#include <Hollow/Events/EventSystem.h>

#include "Components/TransformComponent.h"
#include "Entities/GameObject.h"
#include "Systems/RenderSystem.h"
#include "GUI/GUISystem.h"
#include "Systems/MoveSystem.h"
#include "Components/MoveComponent.h"
#include "Components/SelectComponent.h"
#include "Components/RenderableComponent.h"
#include "ProjectSettings.h"
#include "Components/TransformComponent.h"
#include "Entities/Terrain.h"
#include "Components/TerrainData.h"
#include "Entities/Light.h"
#include "Components/LightComponent.h"
#include "Systems/PhysicsSystem.h"
#include "Components/AnimationComponent.h"
#include "Systems/AnimationSystem.h"
#include "Profiler.h"
#include "Components/PlayerComponent.h"
#include "Systems/PlayerSystem.h"
#include "Events/FileChangeEvent.h"
#include "ShaderManager.h"
#include "FileSystemNotifier.h"
#include "PhysicsDebugDraw.h"
#include "Systems/CameraSystem.h"
#include "TextureManager.h"
#include "Systems/ParticleSystem.h"
#include "Systems/NodeUpdateSystem.h"
#include "ExportHelper.h"
#include "Hollow/Import/HollowModelExporter.h"

class Appliaction
{
public:
	Hollow::Core core;
	Hollow::Window* window;
	Hollow::RenderApi* renderer;
	Hollow::Camera camera;

	Profiler profiler;
	Hollow::s_ptr<AnimationSystem> animationSystem;
	Hollow::s_ptr<GUISystem> gui;
	Hollow::s_ptr<RenderSystem> renderSystem;
	Hollow::s_ptr<PlayerSystem> playerSystem;
	Hollow::s_ptr<FileSystemNotifier> fNotifier;
	Hollow::s_ptr<CameraSystem> cameraSystem;
	Hollow::s_ptr<ParticleSystem> particleSystem;
	Hollow::s_ptr<NodeUpdateSystem> nodeUpdateSystem;
	const Hollow::RendererType rendererType = Hollow::RendererType::DirectX;
	int width;
	int height;
public:
	Appliaction()
	{
		initSettings();

		gui = std::make_shared<GUISystem>(window, renderer);
		fNotifier = std::make_shared<FileSystemNotifier>("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders");

		ProjectSettings::startUp<ProjectSettings>();
		PhysicsSystem::startUp();
		TextureManager::startUp();
		ShaderManager::startUp();

		camera.setProjectionValues(80.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100000.0f);
		
		renderSystem = std::make_shared<RenderSystem>(renderer, width, height);
		renderSystem->m_Camera = &camera;
		renderSystem->skyMap = new SkyMap();

		animationSystem = std::make_shared<AnimationSystem>();
		playerSystem = std::make_shared<PlayerSystem>();
		cameraSystem = std::make_shared<CameraSystem>();
		cameraSystem->setCamera(&camera);
		particleSystem = std::make_shared<ParticleSystem>();
		nodeUpdateSystem = std::make_shared<NodeUpdateSystem>();

		Hollow::SystemManager::instance()->addSystem(renderSystem.get());
		Hollow::SystemManager::instance()->addSystem(animationSystem.get());
		Hollow::SystemManager::instance()->addSystem(PhysicsSystem::instance());
		Hollow::SystemManager::instance()->addSystem(playerSystem.get());
		Hollow::SystemManager::instance()->addSystem(cameraSystem.get());
		Hollow::SystemManager::instance()->addSystem(particleSystem.get());
		Hollow::SystemManager::instance()->addSystem(nodeUpdateSystem.get());

		gui->rendererTab.renderSystem = renderSystem.get();

//		ProjectSettings::instance()->load("C:/dev/Hollow Engine/Project1/Project1.json");
		Hollow::DelayedTaskManager::instance()->update();

		Hollow::TaskManager::instance()->add([&]() {
			fNotifier->run();
		});

		PhysicsSystem::instance()->dynamicsWorld->setDebugDrawer(new PhysicsDebugDraw(renderer));

		Hollow::s_ptr<Hollow::Import::Model> model = Hollow::MeshManager::instance()->import("C:/dev/Hollow Engine/Project1/Meshes/scene2.gltf");
		Hollow::s_ptr<Hollow::Export::Model> exModel = ExportHelper::make(model);
		Hollow::HollowModelExporter::startUp();
		Hollow::HollowModelExporter::instance()->export2("C:/dev/Hollow Engine/Project1/Meshes/scene2test.json", exModel);

		Hollow::s_ptr<Hollow::Import::Model> hwModel = Hollow::MeshManager::instance()->import("C:/dev/Hollow Engine/Project1/Meshes/scene2test.json");
		GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
		RenderableComponent* renderable = entity->addComponent<RenderableComponent>(hwModel);
		TransformComponent* transform = entity->addComponent<TransformComponent>();
		entity->addComponent<AnimationComponent>(hwModel);

		if (0) {}
		//init();
	}

	~Appliaction()
	{
		Hollow::RenderApi::shutdown();
		Hollow::Window::shutdown();
	}

	void initSettings()
	{
		if (!Hollow::FileSystem::exists("settings.json")) {
			Hollow::FileSystem::writeToFile("settings.json", "{ \"renderer\": { \"type\": 1 }, \"window\": { \"type\": 1, \"os\": \"win32\", \"width\": 1920, \"height\": 1080 } }");
		}
		//Hollow::FileSystem::writeToFile("settings.json", "{ renderer: { type: 1 }, window: { type: 1, height: 1920, width: 1080 } }");
		auto settings = nlohmann::json::parse(Hollow::FileSystem::getFileContent("settings.json"));

		Hollow::RendererType rendererType = settings["renderer"]["type"].get<Hollow::RendererType>();
		Hollow::WindowType windowType = settings["window"]["type"].get<Hollow::WindowType>();

		width = settings["window"]["width"].get<int>();
		height = settings["window"]["height"].get<int>();

		if (settings["window"]["os"].get<std::string>() == "win32") {
			if (rendererType == Hollow::RendererType::OpenGL) {
				window = Hollow::OGLWin32Window::startUp<Hollow::OGLWin32Window>(GetModuleHandle(NULL), width, height, windowType);
				renderer = Hollow::RenderApi::startUp<Hollow::OGLRenderApi>(width, height);
			}
			else if (rendererType == Hollow::RendererType::DirectX) {
				window = Hollow::D3D11Win32Window::startUp<Hollow::D3D11Win32Window>(GetModuleHandle(NULL), width, height, windowType);
				renderer = Hollow::RenderApi::startUp<Hollow::D3D11RenderApi>(width, height);
			}
		}
	}

	void init()
	{
		/* PLANE 1 */
		{
			Hollow::s_ptr<Hollow::Import::Model> planeMesh = Hollow::MeshManager::instance()
				->import("C:/dev/Hollow Engine/Project1/Meshes/block.gltf");
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>(planeMesh);
			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>(Hollow::Vector3(0.0f, 25.0f, 0.0f), 0.0f);

			physics->addAABBShape(planeMesh);
			//physics->shape->setLocalScaling(btVector3(2.0f, 2.0f, 2.0f));
			physics->init();

			physics->body->setFriction(1);
			physics->body->setRestitution(0);
			PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			//transform->scale = Hollow::Vector3(2.0f, 2.0f, 2.0f);
		}
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			ParticleComponent* particleComponent = entity->addComponent<ParticleComponent>();
			particleComponent->lifetime = 10.0f;
			particleComponent->maxParticles = 10;
		}
	}

	void update()
	{
		while (!window->isClosed()) {
			core.preUpdate();

			window->processMessage();
			core.update();

			gui->update(core.dt);

			core.postUpdate();
		}
	}
};