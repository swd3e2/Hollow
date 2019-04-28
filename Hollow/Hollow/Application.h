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
//#include "Hollow/ImGuiLayer.h"
#include "LayerStack.h"
#include "Graphics/Camera.h"
#include "Graphics/ForwardRenderSystem.h"
//#include "Common/SaveHelper.h"
#include "Graphics/Camera.h"
#include "Graphics/Renderer/OpenGL/OGLRenderer.h"
#include "Graphics/Renderer/DirectX/D3D11RenderApi.h"
#include "Graphics/Window/Win32/Win32Window.h"
#include "Core/TaskManager.h"

#define SCREEN_WIDTH 2560
#define SCREEN_HEIGHT 1440

class HOLLOW_API Application
{
protected:
	HWND*							m_HWND;

	EntityManager					entityManager;
	ComponentManager				componentManager;
	Hollow::SystemManager           systemManager;
	Timer							m_Timer;
	EventSystem						eventSystem;
	LayerStack						m_LayerStack;
	Camera*							camera;
	//MeshManager						meshManager;
	InputManager					inputManager;
	ForwardRenderSystem*			renderPass;
	//SaveHelper						saveHelper;
	TaskManager						taskManager;
	std::shared_ptr<Win32Window>	window;
	RenderApi* m_Renderer;
	double dt;
	static Application* _instance;
public:
	Application()
	{
		if (_instance == nullptr)
			_instance = this;

		window = std::make_shared<Win32Window>(GetModuleHandle(NULL), SCREEN_WIDTH, SCREEN_HEIGHT);

		/*Hollow::Console::RedirectIOToConsole();
		Hollow::Log::Init();*/

		camera = new Camera(true);
		camera->SetProjectionValues(100.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 10000.0f);

		eventSystem.startUp();
		inputManager.startUp();
		componentManager.startUp();
		entityManager.startUp();
		systemManager.startUp();
		//meshManager.startUp();
		
		m_Renderer = new D3D11RenderApi(SCREEN_WIDTH, SCREEN_HEIGHT, window->getHWND());
		//m_Renderer = new OGLRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, window->getHWND());

		//saveHelper.startUp();

		renderPass = new ForwardRenderSystem(static_cast<D3D11RenderApi*>(m_Renderer));
		renderPass->m_Camera = camera;

		systemManager.AddSystem(renderPass);
		/*ImGuiLayer* layer = new ImGuiLayer((D3DRenderer*)m_Renderer, renderPass, camera);
		layer->window = &*window;
		m_LayerStack.AddLayer(layer);*/


	}

	~Application()
	{
		//saveHelper.shutdown();
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

		while (!window->isClosed())
		{
			dt = m_Timer.GetMilisecondsElapsed();
			m_Timer.Restart();
			window->ProcessMessage();

			//renderPass->shadowMap->camera.Update(dt);
			camera->Update(dt);

			m_LayerStack.PreUpdate(dt);
			systemManager.PreUpdateSystems(dt);

			systemManager.UpdateSystems(dt);
			m_LayerStack.Update(dt);

			m_LayerStack.PostUpdate(dt);
			systemManager.PostUpdateSystems(dt);

			eventSystem.dispatch();
			inputManager.Clear();
			
			m_Timer.Stop();
		}
	}
};