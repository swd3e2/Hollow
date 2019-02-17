#pragma once
#include <iostream>
#include <windows.h>
#include <vector>
#include <queue>
#include <string>

using stringvec = std::vector<std::string>;

void read_directory(const std::string& name, stringvec& v)
{
	std::string pattern(name);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {
			v.push_back(data.cFileName);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}
}

int main()
{
	std::vector<std::string> pathHistory;
	std::string filepath;
	std::string fullFilePath;
	stringvec v;

	while (filepath.c_str() != "exit" && std::cin >> filepath) {
		if (filepath != "." && filepath != "..") {
			if (pathHistory.size() != 0) {
				filepath.append("/");
			} else {
				filepath.append(":/");
			}
			pathHistory.push_back(filepath);
		} else {
			pathHistory.pop_back();
		}
		for (auto& it : pathHistory)
			fullFilePath.append(it);

		v.clear();
		read_directory(fullFilePath, v);
		for (auto& it : v)
			std::cout << it << std::endl;
		fullFilePath.clear();
	}
	
	//Hollow::Console::RedirectIOToConsole();
	//Hollow::Log::Init();
	//Hollow::Engine engine;
	////engine.GetReosourceManager()->LoadFromObj("untitled.obj");
	//std::cin.get();
	return 0;
}

