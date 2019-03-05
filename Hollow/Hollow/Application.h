#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "Events/EventSystem.h"
#include "Common/Timer.h"
#include "Resources/ResourceManager.h"
#include "Input/InputManager.h"
#include "Hollow/Graphics/RenderEngine.h"
#include "Utils/Console.h"
#include "ECS/SystemManager.h"
#include <thread>
#include "DummyConsoleListener.h"
#include "Hollow/ApplicationLayer.h"
#include "Hollow/ImGuiLayer.h"
#include "LayerStack.h"

class HOLLOW_API Application
{
protected:
	float DELTA_TIME_STEP;
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::Timer*					m_Timer;
	EventSystem*					m_EventSystem;
	Hollow::ResourceManager*		m_ResourceManager;
	Hollow::SystemManager*          m_SystemManager;
	IRenderer*						m_Renderer;
	LayerStack						m_LayerStack;
	static Application* _instance;
public:
	Application()
	{
		if (_instance == nullptr)
			_instance = this;

		Hollow::Console::RedirectIOToConsole();
		Hollow::Log::Init();

		m_EventSystem = new EventSystem();
		DummyConsoleListener e;

		m_Renderer = new D3DRenderer();
		m_ComponentManager = new Hollow::ComponentManager();
		m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		m_Timer = new Hollow::Timer();
		m_ResourceManager = new Hollow::ResourceManager();
		m_SystemManager = new Hollow::SystemManager();

		m_LayerStack.AddLayer(new ApplicationLayer((D3DRenderer*)m_Renderer));
		m_LayerStack.AddLayer(new ImGuiLayer((D3DRenderer*)m_Renderer));

		DELTA_TIME_STEP = 1.0f / 60.0f;
	}
public:
	void runRenderer()
	{
		while (!m_Renderer->windowIsClosed()) {
			m_Renderer->PreUpdateFrame();
			m_LayerStack.PreUpdate();
			m_LayerStack.Update();
			m_LayerStack.PostUpdate();
			//InputManager::Clear();
			m_Renderer->PostUpdateFrame();
		}
	}

	void Run()
	{
		std::thread th(&Application::runRenderer, this);

		while (!m_Renderer->windowIsClosed())
		{
			//InputManager::GetMousePosition(m_RenderSystem->GetCamera()->GetProjectionMatrix(), m_RenderSystem->GetCamera()->GetViewMatrix());
			m_Timer->Tick(DELTA_TIME_STEP);
			m_SystemManager->PreUpdateSystems(DELTA_TIME_STEP);
			m_SystemManager->UpdateSystems(DELTA_TIME_STEP);
			m_SystemManager->PostUpdateSystems(DELTA_TIME_STEP);

			m_EventSystem->dispatch();
		}
		th.join();

		/*
		while (m_Renderer->processMessage())
		{
			m_Renderer->PreUpdateFrame();
			
			//InputManager::GetMousePosition(m_RenderSystem->GetCamera()->GetProjectionMatrix(), m_RenderSystem->GetCamera()->GetViewMatrix());
			m_Timer->Tick(DELTA_TIME_STEP);

			m_LayerStack.PreUpdate();
			m_SystemManager->PreUpdateSystems(DELTA_TIME_STEP);

			m_SystemManager->UpdateSystems(DELTA_TIME_STEP);
			m_LayerStack.Update();

			m_SystemManager->PostUpdateSystems(DELTA_TIME_STEP);
			m_LayerStack.PostUpdate();

			m_Renderer->PostUpdateFrame();
			InputManager::Clear();

			m_EventSystem->dispatch();
		}
		*/
	}
};