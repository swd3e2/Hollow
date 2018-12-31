#pragma once
#include "Hollow/Window/Window.h"
#include "Entities/GameObject.h"
#include "Hollow/Graphics/SimpleVertex.h"
#include <Hollow/Engine.h>
#include "Hollow/Components/MeshComponent.h"
#include "Components/MoveComponent.h"
#include "Components/PositionComponent.h"
#include "Systems/InterfaceSystem/InterfaceSystem.h"
#include "Systems/MoveSystem.h"
#include "Systems/RenderSystem/RenderSystem.h"
#include "DirectXMath.h"
#include "d3d11.h"
#include <random>
#include "Hollow/Resources/SoundResource.h"
#include "Hollow/Containers/vector.h"
#include <thread>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

using namespace DirectX;

class Application
{
private:
	static constexpr float DELTA_TIME_STEP{ 1.0f / 60.0f };

	Hollow::Window			m_Window;
	HWND*					m_HWND;
	Hollow::Engine			engine;
	RenderSystem*           m_RenderSystem;
	MoveSystem*				m_MoveSystem;
	InterfaceSystem*		m_InterfaceSystem;

	std::vector<GameObject*> gameObjects;
private:
	void InitScene()
	{
		SoundResource* music = engine.m_ResourceManager->CreateSoundResource("Sandbox/Resources/Sounds/2.wav");
		music->Play();

		bool result;

		std::default_random_engine generator;
		std::uniform_int_distribution < int > distribution(-350, 350);
		for (int i = 0; i < 1024; i++) {
			GameObject * object = engine.m_EntityManager->CreateEntity<GameObject>();
			//object->AddComponent<MeshComponent, ID3D11Device*, std::vector<SimpleVertex>*, std::vector<unsigned int>*>(this->m_RenderSystem->GetDevice(), &vertices, &vindices);
			object->AddComponent<PositionComponent, float, float, float, float>((float)distribution(generator), (float)distribution(generator), (float)distribution(generator), .0f);
			object->AddComponent<MoveComponent>();
			gameObjects.push_back(object);
		}

		/*GameObject* object = engine.m_EntityManager->CreateEntity<GameObject>();
		m_ResourceManager.LoadFromObj(object, "Sandbox/Resources/Meshes/cube.obj");*/
	}
public:
	Application(HINSTANCE hInst, LPWSTR pArgs) :
		m_Window(hInst, SCREEN_WIDTH, SCREEN_HEIGHT)
	{
		this->m_HWND = this->m_Window.getHWND();
		this->m_RenderSystem = new RenderSystem(this->m_HWND, SCREEN_WIDTH, SCREEN_HEIGHT);
		this->m_InterfaceSystem = new InterfaceSystem(this->m_HWND, this->m_RenderSystem->GetDevice(), this->m_RenderSystem->GetDeviceContext(), engine.m_EntityManager, engine.m_ComponentManager);
		this->m_MoveSystem = new MoveSystem();

		this->InitScene();
	}

	void Run() {
		while (m_Window.ProcessMessage())
		{
			engine.m_Timer->Tick(DELTA_TIME_STEP);
			this->m_RenderSystem->PreUpdate(DELTA_TIME_STEP);
			this->m_MoveSystem->Update(DELTA_TIME_STEP, gameObjects);
			this->m_RenderSystem->Update(DELTA_TIME_STEP, gameObjects);
			this->m_InterfaceSystem->Update(DELTA_TIME_STEP);
			this->m_RenderSystem->PostUpdate(DELTA_TIME_STEP);

			engine.m_EventHandler->DispatchEvents();
		}
	}

};