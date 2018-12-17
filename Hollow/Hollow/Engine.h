#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/EventHandler.h"
#include "Common/Timer.h"

namespace Hollow {
	class HOLLOW_API Engine
	{
	public:
		Hollow::EntityManager *			m_EntityManager;
		Hollow::ComponentManager *		m_ComponentManager;
		Hollow::Timer*					m_Timer;
		Hollow::EventHandler*			m_EventHandler;
	public:
		Engine();
		~Engine();
	};
}