#pragma once
#include <iostream>
#include "Hollow/Containers/vector.h"
#include "Hollow/Containers/pair.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Utils/Console.h"
#include "Hollow/Engine.h"


int main()
{
	Hollow::Console::RedirectIOToConsole();
	Hollow::Log::Init();
	Hollow::Engine engine;
	//engine.GetReosourceManager()->LoadFromObj("untitled.obj");
	std::cin.get();
	return 0;
}

