#include "Log.h"

namespace Hollow {
	Log::Log()
	{
		// @todo: check double creation
		_instance = this;
		CONSOLE_SCREEN_BUFFER_INFO coninfo;

		// allocate a console for this app
		AllocConsole();

		// set the screen buffer to be big enough to let us scroll text
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
		coninfo.dwSize.Y = maxConsoleLines;
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

		freopen("CONOUT$", "w", stdout);

		// point to console as well
		std::ios::sync_with_stdio();
	}

	Log::~Log()
	{
		FreeConsole();
	}

	void Log::setColor(const int color) const
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, color);
	}

	void Log::print(std::string string)
	{
		string += '\n';
		fwrite(string.c_str(), sizeof(char), string.size(), stdout);
		fflush(stdout);
	}

	std::string Log::getTime()
	{
		time_t rawtime;
		struct tm* timeinfo;
		char buffer[12];

		time(&rawtime);
		timeinfo = localtime(&rawtime);

		strftime(buffer, sizeof(buffer), "[%H:%M:%S] ", timeinfo);

		return buffer;
	}

	Log* Log::_instance = nullptr;
}