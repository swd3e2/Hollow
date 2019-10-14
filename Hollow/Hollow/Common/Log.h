#pragma once

#ifndef HW_LOG_H
#define HW_LOG_H

#include <string>
#include "vendor/spdlog/include/spdlog/fmt/fmt.h"
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#include <ios>
#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <ctime>
#include <utility>

namespace Hollow {
	class Log
	{
	private:
		const int maxConsoleLines = 1000;
		const int mError = 4;		// red
		const int mWarning = 6;	// yellow
		const int mInfo = 3;		// turquoise
		const int mDebug = 2;		// green
		const int mTrace = 5;		// white
		
		static Log* _instance;
	public:
		Log();
		~Log();
		void setColor(const int color) const;
		void print(std::string string);
		std::string getTime();

		template<typename ...Args>
		std::string getFormattedString(const char* string, Args&& ...args)
		{
			fmt::memory_buffer buf;
			fmt::format_to(buf, string, args...);
			std::string res = getTime() + fmt::to_string(buf);
			return res;
		}

		template<typename ...Args>
		void error(const char* string, Args&& ...args)
		{
			setColor(mError);
			print(getFormattedString(string, std::forward<Args>(args)...));
		}

		template<typename ...Args>
		void warn(const char* string, Args&& ...args)
		{
			setColor(mWarning);
			print(getFormattedString(string, std::forward<Args>(args)...));
		}

		template<typename ...Args>
		void info(const char* string, Args&& ...args)
		{
			setColor(mInfo);
			print(getFormattedString(string, std::forward<Args>(args)...));
		}

		template<typename ...Args>
		void debug(const char* string, Args&& ...args)
		{
			setColor(mDebug);
			print(getFormattedString(string, std::forward<Args>(args)...));
		}

		template<typename ...Args>
		void trace(const char* string, Args&& ...args)
		{
			setColor(mTrace);
			print(getFormattedString(string, std::forward<Args>(args)...));
		}
		
		inline static Log* instance() { return _instance; }
	};
}

#define HW_TRACE(...)    Hollow::Log::instance()->trace(__VA_ARGS__)
#define HW_INFO(...)     Hollow::Log::instance()->info(__VA_ARGS__)
#define HW_WARN(...)     Hollow::Log::instance()->warn(__VA_ARGS__)
#define HW_ERROR(...)    Hollow::Log::instance()->error(__VA_ARGS__)
#define HW_DEBUG(...)    Hollow::Log::instance()->debug(__VA_ARGS__)

#endif