#pragma once

#ifndef HW_LOG_H
#define HW_LOG_H

#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "Hollow/Platform.h"

namespace Hollow {
	class Log
	{
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		Log() {}
		~Log() {}

		static void Init()
		{
			spdlog::set_pattern("%^[%T] %n: %v%$");
			s_CoreLogger = spdlog::stdout_color_mt("HOLLOW");
			s_CoreLogger->set_level(spdlog::level::trace);
			s_ClientLogger = spdlog::stdout_color_mt("APP");
			s_ClientLogger->set_level(spdlog::level::trace);
		}

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

#define HW_TRACE(...)    Hollow::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HW_INFO(...)     Hollow::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HW_WARN(...)     Hollow::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HW_ERROR(...)    Hollow::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HW_DEBUG(...)    Hollow::Log::GetCoreLogger()->debug(__VA_ARGS__)

#endif