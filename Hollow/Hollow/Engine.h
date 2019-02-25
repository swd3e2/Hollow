#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/EventHandler.h"
#include "Common/Timer.h"
#include "Resources/ResourceManager.h"
#include "Input/InputManager.h"
#include "Hollow/Graphics/RenderEngine.h"

	class HOLLOW_API Engine
	{
	private:
		EntityManager *			m_EntityManager;
		ComponentManager *		m_ComponentManager;
		Timer*					m_Timer;
		EventHandler*			m_EventHandler;
		ResourceManager*		m_ResourceManager;
		RenderEngine*			m_RenderEngine;
		static Engine*			instance;
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
