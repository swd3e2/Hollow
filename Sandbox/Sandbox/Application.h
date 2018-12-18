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
#include "Hollow/Resources/ResourceManager.h"
#include "DirectXMath.h"
#include "d3d11.h"
#include <random>

#define SCREEN_WIDTH 1600
#define SCREEN_HEIGHT 900

using namespace DirectX;

class Application
{
private:
	static constexpr float DELTA_TIME_STEP{ 1.0f / 60.0f };

	Hollow::Window					m_Window;
	HWND*					m_HWND;
	Hollow::Engine					engine;
	RenderSystem*           m_RenderSystem;
	MoveSystem*				m_MoveSystem;
	InterfaceSystem*		m_InterfaceSystem;
	std::vector<GameObject*> gameObjects;
private:
	void InitScene()
	{
		/*GameObject* object = engine.m_EntityManager->CreateEntity<GameObject>();
		Hollow::ResourceManager::LoadFromObj(object, "Resources/Meshes/cube.obj");*/
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