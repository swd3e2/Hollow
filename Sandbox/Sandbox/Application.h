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
	RenderSystem* renderPass;
	PlayerSystem* playerSystem;
	FileSystemNotifier fNotifier;
	CameraSystem* cameraSystem;
	const Hollow::RendererType rendererType = Hollow::RendererType::DirectX;
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

		camera.setProjectionValues(80.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100000.0f);

		renderPass = new RenderSystem(renderer, width, height);
		renderPass->skyMap = new SkyMap();
		renderPass->m_Camera = &camera;

		animationSystem = new AnimationSystem();
		playerSystem = new PlayerSystem();
		cameraSystem = new CameraSystem();
		cameraSystem->setCamera(&camera);

		Hollow::SystemManager::instance()->addSystem(renderPass);
		Hollow::SystemManager::instance()->addSystem(animationSystem);
		Hollow::SystemManager::instance()->addSystem(PhysicsSystem::instance());
		Hollow::SystemManager::instance()->addSystem(playerSystem);
		Hollow::SystemManager::instance()->addSystem(cameraSystem);

		gui->rendererTab.renderSystem = renderPass;

		//ProjectSettings::instance()->load("C:\\dev\\Hollow Engine\\Project1\\Project1.json");
		Hollow::DelayedTaskManager::instance()->update();

		//Light* light = Hollow::EntityManager::instance()->create<Light>();
		//light->addComponent<LightComponent>();

		Hollow::TaskManager::instance()->add([&]() {
			fNotifier.run();
		});

		PhysicsSystem::instance()->dynamicsWorld->setDebugDrawer(new PhysicsDebugDraw(renderer));

		init();
	}

	void init()
	{
		Hollow::s_ptr<Hollow::Import::Model> planeMesh = Hollow::MeshManager::instance()
			->import("C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Meshes/untitled.gltf");
		/* Physics test */
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			physics->addPlaneShape(Hollow::Vector3(0.0f, 1.0f, 0.0f), 0, Hollow::Vector3(0.0f, -50.0f, 0.0f), 0);
			PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());
		}
		/* PLANE 1 */
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>(planeMesh);
			
			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			Hollow::Vector3 position = Hollow::Vector3(0.0f, -25.0f, 0.0f);

			physics->addBoxShape(Hollow::Vector3(25.0f, 0.001f, 25.0f), position, 0.0f);
			physics->body->setFriction(1);
			physics->body->setRestitution(0);
			PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = position;
			transform->scale = Hollow::Vector3(25.0f, 0.0f, 25.0f);
		}
		/* PLANE 2 */
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>(planeMesh);

			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			Hollow::Vector3 position = Hollow::Vector3(60.0f, -25.0f, 0.0f);
			physics->addBoxShape(Hollow::Vector3(25.0f, 0.001f, 25.0f), position, 0.0f);
			physics->body->setFriction(1);
			physics->body->setRestitution(0);
			PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = position;
			transform->scale = Hollow::Vector3(25.0f, 0.0f, 25.0f);
		}
		/* FOX */
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			Hollow::s_ptr<Hollow::Import::Model> mesh = Hollow::MeshManager::instance()
				->import("C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Meshes/foxbackup.gltf");
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>(mesh);
			AnimationComponent* animation = entity->addComponent<AnimationComponent>(mesh);

			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			physics->applyRotation = false;
			//physics->load(mesh, Hollow::Vector3(0.0f, 0.0f, 0.0f), 1.0f);
			//physics->addBoxShape(Hollow::Vector3(2.4f, 6.4f, 2.2f), Hollow::Vector3(0.0f, 0.0f, 0.0f), 1.0f);
			physics->addCapsuleShape(4.2, 1.5f, Hollow::Vector3(0.0f, 00.0f, 0.0f), 1.0f);
			physics->body->setContactProcessingThreshold(0.01);
			physics->body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));

			PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = Hollow::Vector3(0.0f, 0.0f, 0.0f);
			transform->scale = Hollow::Vector3(100.0f, 100.0f, 100.0f);

			entity->addComponent<PlayerComponent>();
		}
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			Hollow::s_ptr<Hollow::Import::Model> mesh = Hollow::MeshManager::instance()
				->import("C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Meshes/art/scene.gltf");
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>(mesh);
			//AnimationComponent* animation = entity->addComponent<AnimationComponent>(mesh);

			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			//physics->load(mesh, Hollow::Vector3(20.0f, 0.0f, 0.0f), 0.0f);
			physics->addBoxShape(Hollow::Vector3(2.4f, 6.4f, 2.2f), Hollow::Vector3(0.0f, 0.0f, 0.0f), 50.0f);
			PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = Hollow::Vector3(20.0f, 0.0f, 0.0f);
			transform->scale = Hollow::Vector3(0.004f, 0.004f, 0.004f);
		}
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>();

			Hollow::s_ptr<RenderableObject> renderableObject = std::make_shared<RenderableObject>();
			std::tie(renderableObject->vBuffer, renderableObject->iBuffer) = Hollow::getCube();
			renderableObject->material = 0;
			renderable->renderables.push_back(renderableObject);

			Hollow::s_ptr<Hollow::Material> material = std::make_shared<Hollow::Material>();
			material->materialData.color = Hollow::Vector4(0.0f, 0.0f, 1.0f, 1.0f);
			renderable->materials[0] = material;

			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			Hollow::Vector3 position = Hollow::Vector3(3.0f, 25.0f, 0.0f);
			physics->addBoxShape(Hollow::Vector3(1.f, 1.f, 1.f), position, 1.0f);
			PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body.get());

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = position;
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