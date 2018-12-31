#pragma once
#include "Hollow/Utils/Console.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Application.h"
#include "Hollow.h"

static Hollow::Core::Containers::Vector<unsigned int> testContainer;

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Console::RedirectIOToConsole();
	Hollow::Log::Init();
	Application app(hInst, pArgs);

	testContainer.push_back(1);
	testContainer.push_back(2);
	testContainer.push_back(3);
	testContainer.push_back(4);
	testContainer.push_back(5);
	testContainer.push_back(6);

	app.Run();
	return 0;
}