#include "Engine/Engine.h"
#include "Engine/Console.h"
#include "Engine/Log.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Console::RedirectIOToConsole();
	Hollow::Log::Init();
	Engine engine(hInst, pArgs);
	engine.Run();
	return 0;
}
