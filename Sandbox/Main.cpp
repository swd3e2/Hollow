#pragma once
#include "Sandbox/Application.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hInst2, LPWSTR pArgs, INT)
{
	Appliaction app;
	app.update();
	return 0;
}