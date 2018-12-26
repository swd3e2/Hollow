#pragma once
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/EventHandler.h"
#include "Common/Timer.h"
#include "Resources/AudioEngine.h"

namespace Hollow {
	class HOLLOW_API Engine
	{
	public:
		Hollow::EntityManager *			m_EntityManager;
		Hollow::ComponentManager *		m_ComponentManager;
		Hollow::Timer*					m_Timer;
		Hollow::EventHandler*			m_EventHandler;
		Hollow::AudioEngine*			m_AudioEngine;
	public:
		Engine();
		~Engine();
	};
}