#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include "Hollow/Common/Log.h"

namespace Hollow
{
	class FileSystem
	{
	private:
		using stringvec = std::vector<std::string>;
		std::vector<std::string> pathHistory;
		std::string fullFilePath;
		stringvec* v;

		void read_directory(const std::string& name, stringvec* v)
		{
			std::string pattern(name);
			pattern.append("*");
			WIN32_FIND_DATA data;
			HANDLE hFind;
			if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
				do {
					v->push_back(data.cFileName);
				} while (FindNextFile(hFind, &data) != 0);
				FindClose(hFind);
			} else {
				pathHistory.pop_back();
				read_directory(get_current_file_path(), v);
				HW_ERROR("FindFirstFIle failed on path = \"{}\". Error {}\n", pattern.c_str(), GetLastError());
			}
		}
	public:
		FileSystem()
		{
			v = new stringvec();
		}

		stringvec* read_next_directory(std::string filepath)
		{	
			if (filepath != "." && filepath != "..") {
				if (pathHistory.size() == 0) {
					filepath.append(":\\");
				} else {
					filepath.append("\\");
				}
				pathHistory.push_back(filepath);
			}
			else {
				pathHistory.pop_back();
			}

			v->clear();
			read_directory(get_current_file_path(), v);
			return v;
		}

		stringvec* read_directory(std::string filepath)
		{
			v->clear();
			std::string pattern(filepath);
			pattern.append("*");
			WIN32_FIND_DATA data;
			HANDLE hFind;
			if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
				do {
					v->push_back(data.cFileName);
				} while (FindNextFile(hFind, &data) != 0);
				FindClose(hFind);
			}
			return v;
		}

		std::string get_current_file_path()
		{
			fullFilePath.clear();
			for (auto& it : pathHistory)
				fullFilePath.append(it);

			return fullFilePath;
		}
	};

}