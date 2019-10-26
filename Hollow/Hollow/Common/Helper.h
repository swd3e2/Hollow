#pragma once

#ifndef HW_HELPER_H
#define HW_HELPER_H

#include "Hollow/Platform.h"
#include <string>

namespace Hollow {
	class Helper
	{
	public:
		inline static std::wstring toWideString(const char* string)
		{
			std::wstring result;
			
			size_t size = strlen(string) + 1;
			result.reserve(size);

			size_t outSize;
			mbstowcs_s(&outSize, result.data(), size, string, size - 1);

			return result;
		}

		inline static std::string toString(wchar_t* string)
		{
			std::string result;
			size_t size = wcslen(string) + 1;
			result.reserve(size);

			size_t outSize;
			wcstombs_s(&outSize, result.data(), size, string, size - 1);

			return result;
		}

		inline static std::string trimToLastLineEntry(const char* string, char entry)
		{
			std::string result;

			int lastEntry = -1;
			int stringLenght = strlen(string);

			for (int i = 0; i < stringLenght; i++) {
				if (string[i] == entry) {
					lastEntry = i;
				}
			}

			if (lastEntry == -1) {
				return result;
			}

			char* new_string = new char[lastEntry + 2];
			memcpy(new_string, string, lastEntry + 1);
			new_string[lastEntry + 1] = '\0';

			result = new_string;
			delete[] new_string;

			return result;
		}
	};
}

#endif