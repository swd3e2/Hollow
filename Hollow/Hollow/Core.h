#pragma once

#include "Platform.h"

#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/SystemManager.h"
#include "Events/EventSystem.h"

#include "Common/Timer.h"
#include "Input/InputManager.h"

#include "Common/Console.h"
#include <thread>
#include "Core/TaskManager.h"
#include "RenderApiManager.h"
#include "Common/Log.h" 
#include "Importer/FreeImgImporter.h"
#include "WindowManager.h"

#include "Hollow/Core/DelayedTaskManager.h"
#include "Hollow/Resources/MeshManager.h"
#include "Hollow/Core/DelayedTaskManager.h"
#include "Common/Logger.h"

namespace Hollow {
	/**
	 * Core class which contains all core systems and start ups them
	 */
	class Core
	{
	public:
		HWND*			 m_HWND;
		Timer			 m_Timer;
		double			 dt;
	public:
		Core();
		~Core();

		void preUpdate();
		void update();
		void postUpdate();
	};
}