#pragma once
#include "Window/Window.h"
#include "Common/Timer.h"
#include "Engine/ComponentManager.h"
#include "Engine/EntityManager.h"
#include "Entities/GameObject.h"
#include "Components/MeshComponent.h"
#include "Components/PositionComponent.h"
#include "Engine/Graphics/SimpleVertex.h"
#include "EventHandler.h"
#include "Components/MoveComponent.h"
#include "Systems/RenderSystem/RenderSystem.h"
#include "Systems/InterfaceSystem/InterfaceSystem.h"
#include "Systems/MoveSystem.h"
#include <random>
#include "DirectXMath.h"
#include "d3d11.h"
#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

using namespace DirectX;

class Engine
{
private:
	static constexpr float DELTA_TIME_STEP{ 1.0f / 60.0f };

	Window m_Window;
	HWND*							m_HWND;
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::Timer*					m_Timer;
	Hollow::RenderSystem*           m_RenderSystem;
	Hollow::EventHandler*			m_EventHandler;
	Hollow::MoveSystem*				m_MoveSystem;
	Hollow::InterfaceSystem*		m_InterfaceSystem;
	std::vector<Hollow::GameObject*> gameObjects;
private:
	void InitScene()
	{
		std::vector<SimpleVertex> vertices;
		vertices.push_back({ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(-1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) });

		vertices.push_back({ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(-1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) });
		vertices.push_back({ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) });

		std::vector<unsigned int> vindices;
		unsigned int * indices = new unsigned int[36]{
				2, 1, 0,
				2, 0, 3,
				2, 5, 1,
				5, 7, 1,
				4, 2, 3,
				4, 5, 2,
				4, 3, 0,
				4, 0, 6,
				1, 6, 0,
				1, 7, 6,
				5, 6, 7,
				5, 4, 6
		};
		for (int i = 0; i < 36; i++)
			vindices.push_back(indices[i]);
		
		std::default_random_engine generator;
		std::uniform_int_distribution<int> distribution(-10, 10);

		for (int i = 0; i < 10; i++) {
			Hollow::GameObject * object = this->m_EntityManager->CreateEntity<Hollow::GameObject>();
			object->AddComponent<Hollow::MeshComponent, ID3D11Device*, std::vector<SimpleVertex>*, std::vector<unsigned int>*>(this->m_RenderSystem->GetDevice(), &vertices, &vindices);
			object->AddComponent<Hollow::PositionComponent, float, float, float, float>((float)distribution(generator), (float)distribution(generator), (float)distribution(generator), .0f);
			object->AddComponent<Hollow::MoveComponent>();
			gameObjects.push_back(object);
		}
	}
public:
	Engine(HINSTANCE hInst, LPWSTR pArgs) :
		m_Window(hInst, SCREEN_WIDTH, SCREEN_HEIGHT)
	{
		this->m_HWND = this->m_Window.getHWND();
		this->m_ComponentManager = new Hollow::ComponentManager();
		this->m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		this->m_EventHandler = new Hollow::EventHandler();
		this->m_Timer = new Hollow::Timer();
		this->m_RenderSystem = new Hollow::RenderSystem(this->m_HWND, SCREEN_WIDTH, SCREEN_HEIGHT);
		this->m_InterfaceSystem = new Hollow::InterfaceSystem(this->m_HWND, this->m_RenderSystem->GetDevice(), this->m_RenderSystem->GetDeviceContext(), this->m_EntityManager, this->m_ComponentManager);
		this->m_MoveSystem = new Hollow::MoveSystem();

		this->InitScene();
	}

	void Run() { 
		while (m_Window.ProcessMessage())
		{
			m_Timer->Tick(DELTA_TIME_STEP);
			this->m_RenderSystem->PreUpdate(DELTA_TIME_STEP);
			this->m_MoveSystem->Update(DELTA_TIME_STEP, gameObjects);
			this->m_RenderSystem->Update(DELTA_TIME_STEP, gameObjects);
			this->m_InterfaceSystem->Update(DELTA_TIME_STEP);
			this->m_RenderSystem->PostUpdate(DELTA_TIME_STEP);

			m_EventHandler->DispatchEvents();
		}
	}

};