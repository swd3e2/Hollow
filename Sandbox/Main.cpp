#pragma once
#include "Sandbox/Application.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	MyApp app;
	app.onStartUp();
	app.Run();
	return 0;
}