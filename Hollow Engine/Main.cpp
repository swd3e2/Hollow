#include "Engine/Engine.h"
#define NOMINMAX
#include "windows.h"
#include "Test/ManagersTest.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Engine engine(hInst, pArgs);
	engine.Run();

	ManagersTest();

	return 0;
}