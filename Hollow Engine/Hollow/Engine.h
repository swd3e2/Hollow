#pragma once
#include "Platform.h"
#include "Window/Window.h"
#include "Common/Timer.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/EventHandler.h"

class HOLLOW_API Engine
{
public:
	static constexpr float DELTA_TIME_STEP{ 1.0f / 60.0f };
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::Timer*					m_Timer;
	Hollow::EventHandler*			m_EventHandler;
public:
	Engine()
	{
		this->m_ComponentManager = new Hollow::ComponentManager();
		this->m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		this->m_EventHandler = new Hollow::EventHandler();
		this->m_Timer = new Hollow::Timer();
	}
};