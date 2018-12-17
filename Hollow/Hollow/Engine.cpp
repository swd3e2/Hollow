#include "Engine.h"

namespace Hollow {
	Engine::Engine()
	{
		this->m_ComponentManager = new Hollow::ComponentManager();
		this->m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		this->m_EventHandler = new Hollow::EventHandler();
		this->m_Timer = new Hollow::Timer();
	}

	Engine::~Engine()
	{

	}
}