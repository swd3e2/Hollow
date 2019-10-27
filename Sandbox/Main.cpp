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
#include "Hollow/Graphics/InputLayout.h"
#include "Sandbox/Entities/Light.h"
#include "Sandbox/Components/LightComponent.h"
#include "Sandbox/Systems/PhysicsSystem.h"
#include "Sandbox/Components/AnimationComponent.h"
#include "Sandbox/Systems/AnimationSystem.h"
#include "Sandbox/Profiler.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInst2, LPWSTR pArgs, INT)
{
	Hollow::Core core;

	Hollow::RendererType rendererType = Hollow::RendererType::OpenGL;

	Hollow::Window* window = WindowManager::create(rendererType, SCREEN_WIDTH, SCREEN_HEIGHT, Hollow::WindowType::Bordered);
	Hollow::RenderApi* renderer = RenderApiManager::create(rendererType, SCREEN_WIDTH, SCREEN_HEIGHT);
	
	ProjectSettings::startUp<ProjectSettings>();

	Hollow::Camera* camera = new Hollow::Camera(true);
	camera->setProjectionValues(80.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 100000.0f);

	RenderSystem renderPass(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	renderPass.skyMap = new SkyMap();
	renderPass.m_Camera = camera;

	Profiler profiler;
	MoveSystem moveSystem(camera);
	AnimationSystem animationSystem;

	SystemManager::instance()->addSystem(&renderPass);
	SystemManager::instance()->addSystem(&moveSystem);
	SystemManager::instance()->addSystem(&animationSystem);

	GUISystem* gui = new GUISystem(window, renderer);
	gui->rendererTab.renderSystem = &renderPass;

	PhysicsSystem::startUp();

	//ProjectSettings::instance()->load("C:\\dev\\Hollow Engine\\Project1\\Project1.json");
	DelayedTaskManager::instance()->update();

	//Light* light = Hollow::EntityManager::instance()->create<Light>();
	//light->addComponent<LightComponent>();

	SystemManager::instance()->addSystem(PhysicsSystem::instance());

	/* Physics test */
	{
		GameObject* entity = Hollow::EntityManager::instance()->create<GameObject>();
		// Render
		Hollow::s_ptr<Hollow::Import::Model> mesh = Hollow::MeshManager::instance()
			->import("C:/dev/Hollow Engine/Sandbox/Sandbox/Resources/Meshes/untitled.gltf");
		RenderableComponent* renderable = entity->addComponent<RenderableComponent>(mesh);
		TransformComponent* transform = entity->addComponent<TransformComponent>();
		transform->position = Hollow::Vector3(0.0f, -50.0f, 0.0f);
		transform->scale = Hollow::Vector3(50.0f, 0.0f, 50.0f);

		// Physics
		PhysicsComponent* physics = entity->addComponent<PhysicsComponent>();
		btBoxShape* groundShape = new btBoxShape(btVector3(50.f, 01.f, 50.f));
		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -100, 0));
		btScalar mass(0.);

		btVector3 localInertia(0, 0, 0);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, groundShape, localInertia);
		physics->body = new btRigidBody(cInfo);
		//body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

		PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body);
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
		physics->boxShape = new btBoxShape(btVector3(1.f, 1.f, 1.f));
		
		btScalar mass(1.0f);

		btVector3 localInertia(0, 0, 0);
		physics->boxShape->calculateLocalInertia(mass, localInertia);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0.0f, 100.0f, 0.0f));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, physics->boxShape, localInertia);
		physics->body = new btRigidBody(cInfo);
		PhysicsSystem::instance()->dynamicsWorld->addRigidBody(physics->body);

		TransformComponent* transform = entity->addComponent<TransformComponent>();
		transform->position = Hollow::Vector3(0.0f, 50.0f, 0.0f);
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