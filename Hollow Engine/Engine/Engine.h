#pragma once
#include "Window/Window.h"
#include "Timer.h"
#include "Engine/ECS/ComponentManager.h"
#include "Engine/ECS/EntityManager.h"
#include "ECS/Systems/RenderSystem/RenderSystem.h"

class Engine
{
private:
	static constexpr float DELTA_TIME_STEP{ 1.0f / 60.0f };

	Window m_Window;
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::Timer*					m_Timer;

	Hollow::RenderSystem*           m_RenderSystem;
public:
	Engine(HINSTANCE hInst, LPWSTR pArgs) :
		m_Window(hInst, pArgs, 1600, 900)
	{
		this->m_ComponentManager = new Hollow::ComponentManager();
		this->m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		this->m_Timer = new Hollow::Timer();
		this->m_RenderSystem = new Hollow::RenderSystem(m_Window.getHWND(), 1600, 900);
	}

	void Run() { 
		while (m_Window.ProcessMessage())
		{
			m_Timer->Tick(DELTA_TIME_STEP);
			m_RenderSystem->Update(DELTA_TIME_STEP);
		}
	}

};