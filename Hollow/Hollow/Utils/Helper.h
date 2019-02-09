#pragma once
#include "Hollow/Platform.h"
#include <string>

namespace Hollow {
	class HOLLOW_API Helper
	{
	public:
		inline static wchar_t* converToWideChar(char* string)
		{
			const size_t cSize = strlen(string) + 1;
			wchar_t* wc = new wchar_t[cSize];
			mbstowcs(wc, string, cSize);

			return wc;
		}

		inline static wchar_t* converToWideChar(const char* string)
		{
			return converToWideChar((char*)string);
		}
	};

}

