#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/EventHandler.h"
#include "Common/Timer.h"
#include "Resources/ResourceManager.h"
#include "Input/InputManager.h"
#include "Hollow/Graphics/RenderEngine.h"
#include "Utils/Console.h"
#include "ECS/SystemManager.h"
#include <thread>

class HOLLOW_API Application
{
protected:
	float DELTA_TIME_STEP;
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::Timer*					m_Timer;
	Hollow::EventHandler*			m_EventHandler;
	Hollow::ResourceManager*		m_ResourceManager;
	Hollow::SystemManager*          m_SystemManager;
	IRenderer*						m_Renderer;
	static Application* _instance;
public:
	Application()
	{
		if (_instance == nullptr)
			_instance = this;

		Hollow::Console::RedirectIOToConsole();
		Hollow::Log::Init();

		m_Renderer = new D3DRenderer();
		m_ComponentManager = new Hollow::ComponentManager();
		m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		m_EventHandler = new Hollow::EventHandler();
		m_Timer = new Hollow::Timer();
		m_ResourceManager = new Hollow::ResourceManager();
		m_SystemManager = new Hollow::SystemManager();

		DELTA_TIME_STEP = 1.0f / 60.0f;
	}
public:
	void runRenderer()
	{
		while (!m_Renderer->windowIsClosed()) {
			m_Renderer->Update();
			//InputManager::Clear();
		}
	}

	void Run()
	{
		std::thread th(&Application::runRenderer, this);

		while (m_Renderer->processMessage())
		{
			//InputManager::GetMousePosition(m_RenderSystem->GetCamera()->GetProjectionMatrix(), m_RenderSystem->GetCamera()->GetViewMatrix());
			m_Timer->Tick(DELTA_TIME_STEP);
			m_SystemManager->PreUpdateSystems(DELTA_TIME_STEP);
			m_SystemManager->UpdateSystems(DELTA_TIME_STEP);
			m_SystemManager->PostUpdateSystems(DELTA_TIME_STEP);

			m_EventHandler->DispatchEvents();
		}
		th.join();
	}
};