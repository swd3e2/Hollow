#pragma once
#include <exception>
#include <windows.h>
#include "Hollow/Platform.h"

namespace Hollow {

	class HOLLOW_API HollowRenderer
	{
	protected:
		int width;
		int height;
		HWND* hwnd;

		static HollowRenderer* instance;
	public:
		HollowRenderer(HWND* hwnd, UINT width, UINT height);
		virtual ~HollowRenderer() { }
		static HollowRenderer* Get();
	};

}
