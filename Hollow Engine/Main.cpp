#include "Engine/Engine.h"
#include "Engine/Utils/Console.h"
#include "Engine/Common/Log.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Console::RedirectIOToConsole();
	Hollow::Log::Init();
	Engine engine(hInst, pArgs);
	engine.Run();
	return 0;
}
