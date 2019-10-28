#pragma once

#include <Hollow/Core.h>
#include "Components/TransformComponent.h"
#include "Entities/GameObject.h"
#include "Systems/RenderSystem.h"
#include "GUI/GUISystem.h"
#include "Systems/MoveSystem.h"
#include "Components/MoveComponent.h"
#include "Components/SelectComponent.h"
#include "Components/RenderableComponent.h"
#include "ProjectSettings.h"
#include "Hollow/ECS/EntityManager.h"
#include "Hollow/ECS/ComponentManager.h"
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

class Appliaction
{
public:
	Hollow::Core core;
	Hollow::RendererType rendererType = Hollow::RendererType::OpenGL;
	int width = 1920;
	int height = 1080;
	Hollow::Window* window;
	Hollow::RenderApi* renderer;
	Hollow::Camera camera;

	Profiler profiler;
	AnimationSystem* animationSystem;
	GUISystem* gui;
	RenderSystem* renderPass;
	PlayerSystem* playerSystem;
public:
	Appliaction()
	{
		window = WindowManager::create(rendererType, width, height, Hollow::WindowType::Bordered);
		renderer = RenderApiManager::create(rendererType, width, height);
		gui = new GUISystem(window, renderer);

		ProjectSettings::startUp<ProjectSettings>();
		PhysicsSystem::startUp();

		camera.setProjectionValues(80.0f, static_cast<float>(width) / static_cast<float>(height), 0.1f, 100000.0f);

		renderPass = new RenderSystem(renderer, width, height);
		renderPass->skyMap = new SkyMap();
		renderPass->m_Camera = &camera;

		animationSystem = new AnimationSystem();
		playerSystem = new PlayerSystem();

		SystemManager::instance()->addSystem(renderPass);
		SystemManager::instance()->addSystem(animationSystem);
		SystemManager::instance()->addSystem(PhysicsSystem::instance());
		SystemManager::instance()->addSystem(playerSystem);

		gui->rendererTab.renderSystem = renderPass;

		//ProjectSettings::instance()->load("C:\\dev\\Hollow Engine\\Project1\\Project1.json");
		DelayedTaskManager::instance()->update();

		//Light* light = Hollow::EntityManager::instance()->create<Light>();
		//light->addComponent<LightComponent>();

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

			/*RenderableObject* renderableObject = new RenderableObject();
			std::tie(renderableObject->vBuffer, renderableObject->iBuffer) = Hollow::getCube();
			renderableObject->material = 0;
			renderable->renderables.push_back(renderableObject);

			Hollow::Material* material = new Hollow::Material();
			material->materialData.color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
			renderable->materials[0] = material;*/

			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			//physics->load(mesh, Hollow::Vector3(0.0f, 0.0f, 0.0f), 1.0f);
			//physics->addBoxShape(Hollow::Vector3(2.4f, 6.4f, 2.2f), Hollow::Vector3(0.0f, 00.0f, 0.0f), 20.0f);
			physics->addCapsuleShape(6.0, 2.5f, Hollow::Vector3(0.0f, 00.0f, 0.0f), 20.0f);

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = Hollow::Vector3(0.0f, 0.0f, 0.0f);
			transform->scale = Hollow::Vector3(100.0f, 100.0f, 100.0f);
			transform->rotation = Hollow::Vector3(Hollow::Math::PI / 2, 0, 0);

			entity->addComponent<PlayerComponent>();
		}
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			Hollow::s_ptr<Hollow::Import::Model> mesh = Hollow::MeshManager::instance()
				->import("C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Meshes/foxbackup.gltf");
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>(mesh);

			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			//physics->load(mesh, Hollow::Vector3(20.0f, 0.0f, 0.0f), 0.0f);
			physics->addBoxShape(Hollow::Vector3(2.4f, 6.4f, 2.2f), Hollow::Vector3(0.0f, 0.0f, 0.0f), 20.0f);

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = Hollow::Vector3(20.0f, 0.0f, 0.0f);
			transform->scale = Hollow::Vector3(100.0f, 100.0f, 100.0f);
			transform->rotation = Hollow::Vector3(Hollow::Math::PI / 2, 0, 0);
		}
		{
			GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
			// Render
			RenderableComponent* renderable = entity->addComponent<RenderableComponent>();

			RenderableObject* renderableObject = new RenderableObject();
			std::tie(renderableObject->vBuffer, renderableObject->iBuffer) = Hollow::getCube();
			renderableObject->material = 0;
			renderable->renderables.push_back(renderableObject);

			Hollow::Material* material = new Hollow::Material();
			material->materialData.color = Vector4(0.0f, 0.0f, 1.0f, 1.0f);
			renderable->materials[0] = material;

			// Physics
			PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
			Hollow::Vector3 position = Hollow::Vector3(3.0f, 25.0f, 0.0f);
			physics->addBoxShape(Hollow::Vector3(1.f, 1.f, 1.f), position, 1.0f);

			TransformComponent* transform = entity->addComponent<TransformComponent>();
			transform->position = position;
		}
		/* Animation test */
		//{
		//	GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
		//	Hollow::s_ptr<Hollow::Import::Model> mesh = Hollow::MeshManager::instance()
		//		->import("C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Meshes/scene.gltf");
		//  RenderableComponent* renderable = entity->addComponent<RenderableComponent>(mesh);
		//	AnimationComponent* animation = entity->addComponent<AnimationComponent>(mesh);
		//	TransformComponent* transform = entity->addComponent<TransformComponent>();
		//	transform->position = Hollow::Vector3(0.0f, 0.0f, 0.0f);
		//}
	}

	void update()
	{
		while (!window->isClosed()) {
			core.preUpdate();

			window->processMessage();
			camera.update(core.dt);
			core.update();

			gui->update(core.dt);

			core.postUpdate();
		}
	}
};