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
#include "Hollow/WindowManager.h"
#include "Hollow/Graphics/Platform/DirectX/D3D11Win32Window.h"
#include "Hollow/Graphics/Platform/OpenGL/Win32/OGLWin32Window.h"
#include <Commdlg.h>
#include "Shlobj.h"

namespace Hollow {
	class FileSystem
	{
	public:
		FileSystem() = default;

		std::vector<std::string> readNextDirectory(std::string filepath)
		{	
			std::vector<std::string> v;

			if (filepath != "." && filepath != "..") {
				if (m_PathHistory.size() == 0) {
					filepath.append(":\\");
				} else {
					filepath.append("\\");
				}
				m_PathHistory.push_back(filepath);
			}
			else {
				m_PathHistory.pop_back();
			}

			v.clear();
			readDirectory(getCurrentFilePath(), v);
			return v;
		}

		std::vector<std::string> readDirectory(const std::string& filepath)
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

		std::string getCurrentFilePath()
		{
			m_FullFilePath.clear();
			for (auto& it : m_PathHistory)
				m_FullFilePath.append(it);

			return m_FullFilePath;
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

			return content;
		}

		static void writeToFile(const std::string& filename, const char* content)
		{
			std::ofstream fileStream(filename, std::ios::out);
			fileStream << content;
			fileStream.close();
		}

		static std::string openFile(const std::string& filter)
		{
			OPENFILENAMEA ofn;         // common dialog box structure
			CHAR szFile[260] = { 0 };  // if using TCHAR macros

			// Initialize OPENFILENAME
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = (static_cast<OGLWin32Window*>(Window::instance()))->getHWND();
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

		static std::string openFolder()
		{
			CHAR szFile[260] = { 0 };  // if using TCHAR macros
			LPITEMIDLIST pidl;

			BROWSEINFOA desc = { 0 };
			desc.hwndOwner = (static_cast<OGLWin32Window*>(Window::instance()))->getHWND();
			desc.lpszTitle = "select folder";
			desc.pszDisplayName = szFile;
			pidl = SHBrowseForFolder(&desc);

			if (pidl) {
				SHGetPathFromIDList(pidl, szFile);
				return szFile;
			}

			return std::string();
		}

		static bool createFolder(std::string path)
		{
			return CreateDirectoryA(path.c_str(), NULL);
		}

		static bool copy(const std::string& filepath, const std::string& dst)
		{
			return CopyFile(filepath.c_str(), dst.c_str(), false);
		}

		static bool exists(const std::string& name) 
		{
			std::ifstream f(name.c_str());
			return f.good();
		}
	private:
		std::vector<std::string> m_PathHistory;
		std::string m_FullFilePath;
	private:
		void readDirectory(const std::string& name, std::vector<std::string>& v)
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
			}
			else {
				m_PathHistory.pop_back();
				readDirectory(getCurrentFilePath(), v);
				HW_ERROR("FindFirstFIle failed on path = \"{}\". Error {}\n", pattern.c_str(), GetLastError());
			}
		}
	};
}

#endif