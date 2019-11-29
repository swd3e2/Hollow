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

class Appliaction
{
public:
	Hollow::Core core;
	Hollow::Window* window;
	Hollow::RenderApi* renderer;
	Hollow::Camera camera;

	Profiler profiler;
	AnimationSystem* animationSystem;
	GUISystem* gui;
	RenderSystem* renderSystem;
	PlayerSystem* playerSystem;
	FileSystemNotifier fNotifier;
	CameraSystem* cameraSystem;
	ParticleSystem* particleSystem;
	const Hollow::RendererType rendererType = Hollow::RendererType::OpenGL;
	const int width = 1920;
	const int height = 1080;
public:
	Appliaction() :
		fNotifier("C:/dev/Hollow Engine/Hollow/Hollow/Data/Shaders")
	{
		window = Hollow::WindowManager::create(rendererType, width, height, Hollow::WindowType::Bordered);
		renderer = Hollow::RenderApiManager::create(rendererType, width, height);
	
		gui = new GUISystem(window, renderer);

		ProjectSettings::startUp<ProjectSettings>();
		PhysicsSystem::startUp();
		TextureManager::startUp();
		ShaderManager::startUp();

		camera.setProjectionValues(80.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100000.0f);
		
		renderSystem = new RenderSystem(renderer, width, height);
		renderSystem->m_Camera = &camera;
		renderSystem->skyMap = new SkyMap();

		animationSystem = new AnimationSystem();
		playerSystem = new PlayerSystem();
		cameraSystem = new CameraSystem();
		cameraSystem->setCamera(&camera);
		particleSystem = new ParticleSystem();

		Hollow::SystemManager::instance()->addSystem(renderSystem);
		Hollow::SystemManager::instance()->addSystem(animationSystem);
		Hollow::SystemManager::instance()->addSystem(PhysicsSystem::instance());
		Hollow::SystemManager::instance()->addSystem(playerSystem);
		Hollow::SystemManager::instance()->addSystem(cameraSystem);
		Hollow::SystemManager::instance()->addSystem(particleSystem);

		gui->rendererTab.renderSystem = renderSystem;

		ProjectSettings::instance()->load("C:/dev/Hollow Engine/Project1/Project1.json");
		Hollow::DelayedTaskManager::instance()->update();

		Hollow::TaskManager::instance()->add([&]() {
			fNotifier.run();
		});

		PhysicsSystem::instance()->dynamicsWorld->setDebugDrawer(new PhysicsDebugDraw(renderer));

		//init();
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