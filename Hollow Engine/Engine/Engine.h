#pragma once
#include "Window/Window.h"
#include "Common/Timer.h"
#include "Engine/ComponentManager.h"
#include "Engine/EntityManager.h"
#include "Systems/RenderSystem/RenderSystem.h"
#include "Entities/GameObject.h"
#include "Components/MeshComponent.h"
#include "Components/PositionComponent.h"
#include "Engine/Graphics/SimpleVertex.h"
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
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::Timer*					m_Timer;
	Hollow::RenderSystem*           m_RenderSystem;
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
		{
			vindices.push_back(indices[i]);
		}

		Hollow::GameObject * object = this->m_EntityManager->CreateEntity<Hollow::GameObject>();
		//object->AddComponent<Hollow::MeshComponent, ID3D11Device*, std::vector<SimpleVertex>*, std::vector<unsigned int>*>(this->m_RenderSystem->GetDevice(), &vertices, &vindices);
		//object->AddComponent<Hollow::PositionComponent, float, float, float, float>(2.0f, 1.0f, 0.0, .0f);
		gameObjects.push_back(object);
		
		object = this->m_EntityManager->CreateEntity<Hollow::GameObject>();
		//object->AddComponent<Hollow::MeshComponent, ID3D11Device*, std::vector<SimpleVertex>*, std::vector<unsigned int>*>(this->m_RenderSystem->GetDevice(), &vertices, &vindices);
		//object->AddComponent<Hollow::PositionComponent, float, float, float, float>(3.0f, 4.0f, 1.0, .0f);
		gameObjects.push_back(object);
		m_EntityManager->DestroyEntity<Hollow::GameObject>(object->GetEntityID());
		object = this->m_EntityManager->CreateEntity<Hollow::GameObject>();
		object = this->m_EntityManager->CreateEntity<Hollow::GameObject>();

	}
public:
	Engine(HINSTANCE hInst, LPWSTR pArgs) :
		m_Window(hInst, pArgs, SCREEN_WIDTH, SCREEN_HEIGHT)
	{
		this->m_ComponentManager = new Hollow::ComponentManager();
		this->m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		this->m_Timer = new Hollow::Timer();
		this->m_RenderSystem = new Hollow::RenderSystem(m_Window.getHWND(), 1600, 900);
		this->InitScene();
	}

	void Run() { 
		while (m_Window.ProcessMessage())
		{
			m_Timer->Tick(DELTA_TIME_STEP);
			m_RenderSystem->PreUpdate(DELTA_TIME_STEP);
			m_RenderSystem->Update(DELTA_TIME_STEP, gameObjects);
			m_RenderSystem->PostUpdate(DELTA_TIME_STEP);
		}
	}

};