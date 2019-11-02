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
			size_t size = strlen(string) + 1;
			std::wstring result(size, 0);

			size_t outSize;
			mbstowcs_s(&outSize, result.data(), size, string, size - 1);

			return result;
		}

		inline static std::string toString(wchar_t* string)
		{
			size_t size = (wcslen(string) / 2);
			return toString(string, size);
		}

		inline static std::string toString(wchar_t* string, int size)
		{
			size += 1;
			std::string result(size, 0);

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

		template<typename T>
		inline static void swap(T& first, T& second)
		{
			T temp = first;
			first = second;
			second = temp;
		}
	};
}

#endif