#pragma once
#define D3D11

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
#include "Layers/LayerStack.h"
#include "Graphics/Camera.h"
#include "Graphics/ForwardRenderSystem.h"
//#include "Common/SaveHelper.h"
#include "Graphics/Camera.h"
#include "Graphics/WindowManager.h"
#include "Core/TaskManager.h"
#include "Graphics/RenderApiManager.h"
#include "Graphics/Renderer/OpenGL/OGLRenderApi.h"
#include "Common/Log.h"
#include "Graphics/SkyMap.h"

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
	InputManager					inputManager;
	ForwardRenderSystem*			renderPass;
	TaskManager						taskManager;
	RenderApiManager				renderApiManager;
	Window*							window;
	RenderApi*						m_Renderer;
	double							dt;
public:
	Application();
	~Application();

	void Run();
};