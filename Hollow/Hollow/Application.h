#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "Events/EventSystem.h"
#include "Common/Timer.h"
#include "Resources/ResourceManager.h"
#include "Input/InputManager.h"
#include "Utils/Console.h"
#include "ECS/SystemManager.h"
#include <thread>
#include "DummyConsoleListener.h"
#include "Hollow/ImGuiLayer.h"
#include "LayerStack.h"
#include "Graphics/Camera.h"
#include "SceneManager.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class HOLLOW_API Application
{
protected:
	float DELTA_TIME_STEP;
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::ResourceManager*		m_ResourceManager;
	Hollow::SystemManager*          m_SystemManager;
	Timer							m_Timer;
	EventSystem*					m_EventSystem;
	IRenderer*						m_Renderer;
	LayerStack						m_LayerStack;
	InputManager*					m_InputManager;
	Camera*							camera;
	SceneManager					sceneManager;
	double dt;
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
		camera = new Camera();
		camera->SetProjectionValues(75.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 1000.0f);

		m_Renderer = new D3DRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);
		m_Renderer->SetCamera(camera);

		m_ComponentManager = new Hollow::ComponentManager();
		m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		m_ResourceManager = new Hollow::ResourceManager();
		m_SystemManager = new Hollow::SystemManager();
		m_InputManager = new InputManager();

		m_LayerStack.AddLayer(new ImGuiLayer((D3DRenderer*)m_Renderer, sceneManager.GetSceneObjects()));
		
		sceneManager.StartUp();
		DELTA_TIME_STEP = 1.0f / 60.0f;
	}

	~Application()
	{
		sceneManager.Shutdown();
	}
public:

	void Run()
	{
		m_Timer.Start();

		while (!m_Renderer->windowIsClosed())
		{
			dt = m_Timer.GetMilisecondsElapsed();
			m_Timer.Restart();
			m_Renderer->processMessage();

			camera->Update(dt);
			m_Renderer->PreUpdateFrame();

			m_SystemManager->PreUpdateSystems(DELTA_TIME_STEP);
			m_LayerStack.PreUpdate();

			m_LayerStack.Update();
			m_SystemManager->UpdateSystems(DELTA_TIME_STEP);
			m_Renderer->Update(sceneManager.GetSceneObjects());

			m_SystemManager->PostUpdateSystems(DELTA_TIME_STEP);
			m_LayerStack.PostUpdate();

			m_EventSystem->dispatch();
			InputManager::Clear();
			m_Renderer->PostUpdateFrame();

			m_Timer.Stop();
		}
	}
};