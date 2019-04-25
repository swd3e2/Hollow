#ifdef HOLLOW_BUILD_DLL
	#define HOLLOW_API __declspec(dllexport)
	#define IMGUI_API __declspec( dllexport )
#else 
	#define HOLLOW_API __declspec(dllimport)
	#define IMGUI_API __declspec( dllimport )
#endif

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#ifdef D3D11
#pragma comment( lib, "d3d11.lib")
#pragma comment( lib, "D3DCompiler.lib")
#endif

#ifdef OPENGL
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#endif


using UINT = unsigned int;