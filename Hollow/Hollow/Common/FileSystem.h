#pragma once

#ifndef HW_FILE_SYSTEM_H
#define HW_FILE_SYSTEM_H

#include <iostream>
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include <vector>
#include <string>
#include "Log.h"
#include "Hollow/Platform.h"
#include <fstream>

namespace Hollow
{
	class  FileSystem
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

		static std::string getFileContent(const std::string& filepath)
		{
			std::string content;
			std::ifstream fileStream(filepath, std::ios::in);

			if (!fileStream.is_open()) {
				HW_DEBUG("FileSystem: can't open file {}", filepath.c_str());
				return "";
			}

			std::string line = "";
			while (!fileStream.eof()) {
				std::getline(fileStream, line);
				content.append(line + "\n");
			}

			fileStream.close();
			std::cout << "'" << content << "'" << std::endl;
			return content.c_str();
		}
	};

}

#endif