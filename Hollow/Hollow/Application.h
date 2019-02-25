#pragma once
#include "Engine.h"
#include "Platform.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include "ECS/EventHandler.h"
#include "Common/Timer.h"
#include "Resources/ResourceManager.h"
#include "Input/InputManager.h"
#include "Hollow/Graphics/RenderEngine.h"

class HOLLOW_API Application
{
private:
	Hollow::EntityManager *			m_EntityManager;
	Hollow::ComponentManager *		m_ComponentManager;
	Hollow::Timer*					m_Timer;
	Hollow::EventHandler*			m_EventHandler;
	Hollow::ResourceManager*		m_ResourceManager;
	Hollow::RenderEngine*			m_RenderEngine;
	Engine* engine;
	static Application* _instance;
public:
	Application(HWND* hwnd, int width, int height)
	{
		if (_instance == nullptr)
			_instance = this;

		this->m_ComponentManager = new Hollow::ComponentManager();
		this->m_EntityManager = new Hollow::EntityManager(m_ComponentManager);
		this->m_EventHandler = new Hollow::EventHandler();
		this->m_Timer = new Hollow::Timer();
		this->m_ResourceManager = new Hollow::ResourceManager();
		this->m_RenderEngine = new Hollow::RenderEngine(hwnd, width, height);
	}
};