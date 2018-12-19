#pragma once
#include "Hollow/Utils/Console.h"
#include "Hollow/Common/Log.h"
#include "Sandbox/Application.h"
#include "Hollow.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Console::RedirectIOToConsole();
	Hollow::Log::Init();
	Application app(hInst, pArgs);
	app.Run();
	return 0;
}