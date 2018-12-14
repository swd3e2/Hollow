#include "Engine/Engine.h"
#include "Engine/Utils/Console.h"
#include "Engine/Common/Log.h"
#include "Engine/Utils/ObjParser.h"

// App entrypoint
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR pArgs, INT)
{
	Hollow::Console::RedirectIOToConsole();
	Hollow::Log::Init();
	Hollow::ObjParser::LoadObj("cube.obj");
	system("pause");
	/*Engine engine(hInst, pArgs);
	engine.Run();*/
	return 0;
}
