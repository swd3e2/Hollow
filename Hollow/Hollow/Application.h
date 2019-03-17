#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "Events/EventSystem.h"
#include "Common/Timer.h"
#include "Input/InputManager.h"
#include "Common/Console.h"
#include "ECS/SystemManager.h"
#include <thread>
#include "Hollow/ImGuiLayer.h"
#include "LayerStack.h"
#include "Graphics/Camera.h"
#include "SceneManager.h"
#include "Resources/TextureManager.h"
#include "Resources/ShaderManager.h"
#include "Resources/MeshManager.h"
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class HOLLOW_API Application
{
protected:
	HWND*							m_HWND;

	Hollow::EntityManager			entityManager;
	Hollow::ComponentManager		componentManager;
	Hollow::SystemManager           systemManager;
	Timer							m_Timer;
	EventSystem						eventSystem;
	D3DRenderer*					m_Renderer;
	LayerStack						m_LayerStack;
	Camera*							camera;
	SceneManager					sceneManager;
	TextureManager					textureManager;
	ShaderManager					shaderManager;
	MeshManager						meshManager;
	InputManager					inputManager;
	double dt;
	static Application* _instance;
public:
	Application()
	{
		if (_instance == nullptr)
			_instance = this;

		Hollow::Console::RedirectIOToConsole();
		Hollow::Log::Init();

		camera = new Camera();
		camera->SetProjectionValues(75.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 1000.0f);

		eventSystem.startUp();
		inputManager.startUp();
		componentManager.startUp();
		entityManager.startUp(&componentManager);
		sceneManager.startUp();
		systemManager.startUp();
		
		meshManager.startUp();

		m_Renderer = new D3DRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);
		m_Renderer->SetCamera(camera);
		
		textureManager.startUp(m_Renderer->getDevice(), m_Renderer->getDeviceContext());
		shaderManager.startUp(m_Renderer->getDevice());

		m_LayerStack.AddLayer(new ImGuiLayer((D3DRenderer*)m_Renderer, sceneManager.GetSceneObjects()));
	}

	~Application()
	{
		meshManager.shutdown();
		shaderManager.shutdown();
		textureManager.shutdown();
		sceneManager.shutdown();
		systemManager.shutdown();
		entityManager.shutdown();
		componentManager.shutdown();
		inputManager.shutdown();
		eventSystem.shutdown();
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

			systemManager.PreUpdateSystems(dt);
			m_LayerStack.PreUpdate(dt);

			m_LayerStack.Update(dt);
			systemManager.UpdateSystems(dt);
			m_Renderer->Update(sceneManager.GetSceneObjects());

			systemManager.PostUpdateSystems(dt);
			m_LayerStack.PostUpdate(dt);

			eventSystem.dispatch();
			inputManager.Clear();

			m_Renderer->PostUpdateFrame();

			m_Timer.Stop();
		}
	}
};