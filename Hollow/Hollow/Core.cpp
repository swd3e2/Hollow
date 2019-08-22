#include "Core.h"

namespace Hollow {
	Core::Core()
	{
		Console::RedirectIOToConsole();
		Log::Init();

		Logger::startUp();
		FreeImgImporter::startUp();
		TaskManager::startUp();
		MeshManager::startUp();
		DelayedTaskManager::startUp();
		EventSystem::startUp();
		InputManager::startUp();
		ComponentManager::startUp();
		EntityManager::startUp();
		SystemManager::startUp();

		m_Timer.start();
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
		/* @todo: check fix error */
		//TaskManager::shutdown();
		FreeImgImporter::shutdown();
		Logger::shutdown();
	}

	void Core::preUpdate()
	{
		dt = m_Timer.getMilisecondsElapsed();
		m_Timer.restart();

		SystemManager::instance()->preUpdateSystems(dt);
		EventSystem::instance()->dispatch();
	}

	void Core::update()
	{
		SystemManager::instance()->updateSystems(dt);
	}

	void Core::postUpdate()
	{
		SystemManager::instance()->postUpdateSystems(dt);
		InputManager::instance()->clear();

		m_Timer.stop();
		DelayedTaskManager::instance()->update();
	}
}

