#pragma once

#ifndef HW_HELPER_H
#define HW_HELPER_H

#include "Hollow/Platform.h"
#include <string>

namespace Hollow {
	class Helper
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

		inline static char* trim_to_last_line_entry(const char* string, char entry)
		{
			int lastEntry = -1;
			int prevEntry = -1;
			int stringLenght = strlen(string);

			for (int i = 0; i < stringLenght; i++) {
				if (string[i] == entry) {
					lastEntry = i;
				}
			}
			if (lastEntry == -1) return nullptr;

			char* new_string = new char[lastEntry + 2];
			memcpy((void*)new_string, (void*)string, lastEntry + 2);
			new_string[lastEntry + 1] = '\0';
			return new_string;
		}

		inline static char* trim_to_symbol(const char* string, char entry)
		{
			int lastEntry = -1;
			for (int i = 0; i < strlen(string); i++) {
				if (string[i] == entry) {
					lastEntry = i;
				}
			}
			if (lastEntry == -1) return nullptr;

			char* new_string = new char[lastEntry+1];
			memcpy((void*)new_string, (void*)string, lastEntry + 1);
			new_string[lastEntry] = '\0';
			return new_string;
		}
	};
}

#endif