#include "Core.h"

namespace Hollow {
	Core::Core(RendererType type) :
		windowManager(type), renderApiManager(type)
	{
		Console::RedirectIOToConsole();
		Log::Init();

		FreeImgImporter::startUp();
		TaskManager::startUp();
		MeshManager::startUp();
		DelayedTaskManager::startUp();
		EventSystem::startUp();
		InputManager::startUp();
		ComponentManager::startUp();
		EntityManager::startUp();
		SystemManager::startUp();

		m_Timer.Start();
	}

	Core::~Core()
	{
		SystemManager::shutdown();
		EntityManager::shutdown();
		ComponentManager::shutdown();
		InputManager::shutdown();
		EventSystem::shutdown();
		DelayedTaskManager::shutdown();
		MeshManager::shutdown();
		TaskManager::shutdown();
		FreeImgImporter::shutdown();
	}

	void Core::PreUpdate()
	{
		dt = m_Timer.GetMilisecondsElapsed();
		m_Timer.Restart();

		SystemManager::instance()->PreUpdateSystems(dt);
	}

	void Core::Update()
	{
		SystemManager::instance()->UpdateSystems(dt);
	}

	void Core::PostUpdate()
	{
		SystemManager::instance()->PostUpdateSystems(dt);
			
		EventSystem::instance()->dispatch();
		InputManager::instance()->Clear();

		m_Timer.Stop();

		DelayedTaskManager::instance()->Update();
	}
}

