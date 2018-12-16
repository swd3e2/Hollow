#define NOMINMAX

#include "Sandbox/Application.h"
#include "Utils/Console.h"
#include "Common/Log.h"
#include "Utils/ObjParser.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Console::RedirectIOToConsole();
	Hollow::Log::Init();
	//Hollow::ObjParser::LoadObj("cube.obj");
	//system("pause");
	Application app(hInst, pArgs);
	app.Run();
	return 0;
}