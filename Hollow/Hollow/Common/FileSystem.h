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
#include "Hollow/Graphics/WindowManager.h"
#include "Hollow/Graphics/Renderer/DirectX/D3D11Win32Window.h"
#include "Hollow/Graphics/Renderer/OpenGL/Win32/OGLWin32Window.h"
#include <Commdlg.h>
#include "Shlobj.h"

namespace Hollow {
	class FileSystem
	{
	private:
		std::vector<std::string> pathHistory;
		std::string fullFilePath;

		void read_directory(const std::string& name, std::vector<std::string>& v)
		{
			std::string pattern(name);
			pattern.append("*");
			WIN32_FIND_DATA data;
			HANDLE hFind;
			if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
				do {
					v.push_back(data.cFileName);
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
		}

		std::vector<std::string> read_next_directory(std::string filepath)
		{	
			std::vector<std::string> v;

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

			v.clear();
			read_directory(get_current_file_path(), v);
			return v;
		}

		std::vector<std::string> read_directory(const std::string& filepath)
		{
			std::vector<std::string> v;

			std::string pattern(filepath);
			pattern.append("*");
			WIN32_FIND_DATA data;
			HANDLE hFind;
			if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
				do {
					v.push_back(data.cFileName);
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

		static void writeToFile(const std::string& filename, const char* content)
		{
			std::ofstream fileStream(filename, std::ios::out);
			fileStream << content;
			fileStream.close();
		}

		static std::string OpenFile(const std::string& filter)
		{
			OPENFILENAMEA ofn;         // common dialog box structure
			CHAR szFile[260] = { 0 };  // if using TCHAR macros

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = *(static_cast<OGLWin32Window*>(WindowManager::instance()->getWindow()))->getHWND();
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = sizeof(szFile);
			ofn.lpstrFilter = "All\0*.*\0";
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileNameA(&ofn) == TRUE)
			{
				return ofn.lpstrFile;
			}
			return std::string();
		}

		static std::string OpenFolder()
		{
			CHAR szFile[260] = { 0 };  // if using TCHAR macros
			LPITEMIDLIST pidl;

			BROWSEINFOA desc = { 0 };
			desc.hwndOwner = *(static_cast<OGLWin32Window*>(WindowManager::instance()->getWindow()))->getHWND();
			desc.lpszTitle = "select folder";
			desc.pszDisplayName = szFile;
			pidl = SHBrowseForFolder(&desc);

			if (pidl) {
				SHGetPathFromIDList(pidl, szFile);
				return szFile;
			}

			return std::string();
		}

		static bool CreateFolder(std::string path)
		{
			return CreateDirectoryA(path.c_str(), NULL);
		}

		static bool Copy(const std::string& filepath, const std::string& dst)
		{
			return CopyFile(filepath.c_str(), dst.c_str(), false);
		}
	};

}

#endif