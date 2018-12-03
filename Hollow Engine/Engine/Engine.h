#pragma once
#include "Window/Window.h"

class Engine
{
public:
	Engine(HINSTANCE hInst, LPWSTR pArgs) :
		m_Window(hInst, pArgs, 1600, 900)
	{}

	void Run() { 
		while (m_Window.ProcessMessage())
		{
			
		}
	}

	Window					m_Window;
};