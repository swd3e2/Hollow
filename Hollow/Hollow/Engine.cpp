#include "Engine.h"

Engine* Engine::instance = nullptr;

Engine::Engine()
{
	

	InputManager::StartUp();
}

Engine::~Engine()
{
}
