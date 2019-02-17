#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/EventHandler.h"
#include "Common/Timer.h"
#include "Resources/ResourceManager.h"

namespace Hollow {
	class HOLLOW_API Engine
	{
	private:
		Hollow::EntityManager *			m_EntityManager;
		Hollow::ComponentManager *		m_ComponentManager;
		Hollow::Timer*					m_Timer;
		Hollow::EventHandler*			m_EventHandler;
		Hollow::ResourceManager*		m_ResourceManager;
		static Engine* instance;
	public:
		Engine();
		~Engine();

		inline Hollow::EntityManager *		GetEntityManager() { return this->m_EntityManager; };
		inline Hollow::ComponentManager *	GetComponentManager() { return this->m_ComponentManager; };
		inline Hollow::Timer *				GetTimer() { return this->m_Timer; };
		inline Hollow::EventHandler *		GetEventHandler() { return this->m_EventHandler; };
		inline Hollow::ResourceManager *	GetReosourceManager() { return this->m_ResourceManager; };

		inline static Engine* Get() { return instance; }
	};
}