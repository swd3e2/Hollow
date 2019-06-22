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
#include "Layers/LayerStack.h"
#include "Graphics/Camera.h"
#include "Graphics/Camera.h"
#include "Graphics/WindowManager.h"
#include "Core/TaskManager.h"
#include "Graphics/RenderApiManager.h"
#include "Graphics/Renderer/OpenGL/OGLRenderApi.h"
#include "Common/Log.h"
#include "Importer/FreeImgImporter.h"
#include "Graphics/WindowManager.h"
#include "Core/ProjectSettings.h"

namespace Hollow {
	class Core
	{
	public:
		HWND* m_HWND;
		EntityManager					entityManager;
		ComponentManager				componentManager;
		Hollow::SystemManager           systemManager;
		Timer							m_Timer;
		EventSystem						eventSystem;
		LayerStack						m_LayerStack;
		InputManager					inputManager;
		TaskManager						taskManager;
		RenderApiManager				renderApiManager;
		FreeImgImporter					imgImporter;
		WindowManager					windowManager;
		ProjectSettings					projectSettings;
		double							dt;
	public:
		Core(RendererType type);
		~Core();

		void PreUpdate();
		void Update();
		void PostUpdate();
	};
}