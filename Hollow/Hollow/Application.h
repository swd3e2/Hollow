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
#include "Graphics/ForwardRenderPass.h"
#include "Common/SaveHelper.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

class HOLLOW_API Application
{
protected:
	HWND*							m_HWND;

	EntityManager					entityManager;
	ComponentManager				componentManager;
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
	ForwardRenderPass*				renderPass;
	SaveHelper						saveHelper;
	std::shared_ptr<Win32Window>	window;

	double dt;
	static Application* _instance;
public:
	Application()
	{
		if (_instance == nullptr)
			_instance = this;

		window = std::make_shared<Win32Window>(GetModuleHandle(NULL), SCREEN_WIDTH, SCREEN_HEIGHT);

		Hollow::Console::RedirectIOToConsole();
		Hollow::Log::Init();

		camera = new Camera(true);
		camera->SetProjectionValues(75.0f, static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT), 0.1f, 10000.0f);

		eventSystem.startUp();
		inputManager.startUp();
		componentManager.startUp();
		entityManager.startUp();
		sceneManager.startUp();
		systemManager.startUp();
		
		meshManager.startUp();
		
		m_Renderer = new D3DRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, window->getHWND());
		
		textureManager.startUp(m_Renderer->getDevice(), m_Renderer->getDeviceContext());
		shaderManager.startUp(m_Renderer->getDevice());
		saveHelper.startUp();

		renderPass = new ForwardRenderPass(m_Renderer);
		renderPass->m_Camera = camera;
		
		systemManager.AddSystem(renderPass);
		ImGuiLayer* layer = new ImGuiLayer((D3DRenderer*)m_Renderer, renderPass, sceneManager.GetSceneObjects(), camera);
		layer->window = &*window;
		m_LayerStack.AddLayer(layer);
	}

	~Application()
	{
		saveHelper.shutdown();
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

		while (!window->isClosed())
		{
			dt = m_Timer.GetMilisecondsElapsed();
			m_Timer.Restart();
			window->ProcessMessage();

			renderPass->shadowMap->camera.Update(dt);
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