#ifdef HOLLOW_BUILD_DLL
	#define HOLLOW_API __declspec(dllexport)
	#define IMGUI_API __declspec( dllexport )
#else 
	#define HOLLOW_API __declspec(dllimport)
	#define IMGUI_API __declspec( dllimport )
#endif

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

using UINT = unsigned int;
