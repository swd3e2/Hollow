#pragma once

#ifdef HOLLOW_PLATFORM_WIDNOWS
	#ifdef HOLLOW_BUILD_DLL
		#define HOLLOW_API __declspec(dllexport)
	#else 
		#define HOLLOW_API __declspec(dllimport)
	#endif
#endif