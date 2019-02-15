#pragma once
#include "Hollow/Platform.h"
#include <string>

namespace Hollow {
	class HOLLOW_API Helper
	{
	public:
		inline static wchar_t* converToWideChar(const char* string)
		{
			size_t cSize = strlen(string) + 1;
			wchar_t* wc = new wchar_t[cSize];
			size_t outSize;
			mbstowcs_s(&outSize, wc, cSize, string, cSize - 1);
			return wc;
		}
	};

}

