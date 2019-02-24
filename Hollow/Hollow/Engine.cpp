#include "Engine.h"

namespace Hollow {
	Engine* Engine::instance = nullptr;

	Engine::Engine()
	{
		this->m_ComponentManager = new Hollow::ComponentManager();
		this->m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		this->m_EventHandler = new Hollow::EventHandler();
		this->m_Timer = new Hollow::Timer();
		this->m_ResourceManager = new Hollow::ResourceManager();

		instance = this;

		InputManager::StartUp();
	}

	Engine::~Engine()
	{
	}
}