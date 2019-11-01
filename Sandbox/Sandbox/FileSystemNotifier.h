#pragma once

#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <algorithm>
#include "Hollow/Common/Helper.h"
#include "Hollow/Events/EventSystem.h"
#include "Sandbox/Events/FileChangeEvent.h"

class FileSystemNotifier
{
private:
	std::vector<std::string> fileExtensions;
	std::vector<std::string> changedFiles;
	LPCTSTR rootFolder;
public:
	FileSystemNotifier(const char* folder)
	{
		fileExtensions.push_back("hlsl");
		fileExtensions.push_back("glsl");
		rootFolder = folder;
	}

	void sendNotify(PFILE_NOTIFY_INFORMATION pNotify)
	{
		std::string filename = Hollow::Helper::toString(pNotify->FileName, pNotify->FileNameLength / 2);
		std::replace(filename.begin(), filename.end(), '\\', '/');
		std::string extension = filename.substr(filename.size() - 5, filename.size());

		bool validExtension = std::find(fileExtensions.begin(), fileExtensions.end(), extension.c_str()) != fileExtensions.end();
		bool notChanged = std::find(changedFiles.begin(), changedFiles.end(), filename) == changedFiles.end();

		if (validExtension && notChanged) {
			changedFiles.push_back(filename);
		}
	}

	void run()
	{
		HANDLE hDir = CreateFile(rootFolder,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
			NULL,
			OPEN_EXISTING,
			FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
			NULL
		);

		OVERLAPPED o = {};
		o.hEvent = CreateEvent(0, FALSE, FALSE, 0);

		DWORD nBufferLength = 60 * 1024;
		BYTE* lpBuffer = new BYTE[nBufferLength];
		bool bStop = false;

		while (!bStop) {
			int size = changedFiles.size();
			changedFiles.clear();

			DWORD returnedBytes = 0;
			ReadDirectoryChangesW(hDir, lpBuffer, nBufferLength, TRUE,
				FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_LAST_WRITE,
				&returnedBytes, &o, 0);

			DWORD dwWaitStatus = WaitForSingleObject(o.hEvent, INFINITE);

			if (size == 0) {
				switch (dwWaitStatus)
				{
				case WAIT_OBJECT_0: {
					DWORD seek = 0;
					while (seek < nBufferLength) {
						PFILE_NOTIFY_INFORMATION pNotify = PFILE_NOTIFY_INFORMATION(lpBuffer + seek);
						seek += pNotify->NextEntryOffset;

						if (pNotify->Action == FILE_ACTION_RENAMED_NEW_NAME || pNotify->Action == FILE_ACTION_MODIFIED)
							sendNotify(pNotify);

						if (pNotify->NextEntryOffset == 0 || changedFiles.size() > 0)
							break;
					}
				} break;
				default:
					bStop = true;
					break;
				}
			}

			for (auto& it : changedFiles) {
				Hollow::EventSystem::instance()->addEvent(new FileChangeEvent(it), 1);
			}
		}

		CloseHandle(o.hEvent);
		delete[] lpBuffer;
	}
};
