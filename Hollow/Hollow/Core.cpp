#include "Core.h"

namespace Hollow {
	Core::Core(RendererType type) : 
		renderApiManager(type), windowManager(type)
	{
		Hollow::Console::RedirectIOToConsole();
		Hollow::Log::Init();

		eventSystem.startUp();
		inputManager.startUp();
		componentManager.startUp();
		entityManager.startUp();
		systemManager.startUp();

		m_Timer.Start();
	}

	Core::~Core()
	{
		systemManager.shutdown();
		entityManager.shutdown();
		componentManager.shutdown();
		inputManager.shutdown();
		eventSystem.shutdown();
	}

	void Core::PreUpdate()
	{
		dt = m_Timer.GetMilisecondsElapsed();
		m_Timer.Restart();

		systemManager.PreUpdateSystems(dt);
	}

	void Core::Update()
	{
		systemManager.UpdateSystems(dt);
	}

	void Core::PostUpdate()
	{
		systemManager.PostUpdateSystems(dt);
			
		eventSystem.dispatch();
		inputManager.Clear();

		m_Timer.Stop();
	}
}

