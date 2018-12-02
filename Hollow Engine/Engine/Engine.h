#pragma once
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "Window/Window.h"

class Engine
{
public:
	Engine(HINSTANCE hInst, LPWSTR pArgs) :
		m_EntityManager(&m_ComponentManager),
		m_Window(hInst, pArgs, 1600, 900)
	{}

	void Run() { 
		while (m_Window.ProcessMessage())
		{
			
		}
	}

	Window					m_Window;
	ECS::ComponentManager	m_ComponentManager;
	ECS::EntityManager		m_EntityManager;
};