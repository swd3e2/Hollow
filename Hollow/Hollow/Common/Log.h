#pragma once

#ifndef HW_LOG_H
#define HW_LOG_H

#include "Hollow/Platform.h"
#include "Logger.h"

namespace Hollow {
	class Log
	{
	private:
		static std::shared_ptr<Logger> m_CoreLogger;
	public:
		Log() {}
		~Log() {}

		static void Init()
		{
			Logger::startUp();
		}

		inline static std::shared_ptr<Logger>& GetCoreLogger() { return m_CoreLogger; }
	};
}

#define HW_TRACE(...)    Hollow::Logger::instance()->log(__VA_ARGS__)
#define HW_INFO(...)     Hollow::Logger::instance()->log(__VA_ARGS__)
#define HW_WARN(...)     Hollow::Logger::instance()->log(__VA_ARGS__)
#define HW_ERROR(...)    Hollow::Logger::instance()->log(__VA_ARGS__)
#define HW_DEBUG(...)    Hollow::Logger::instance()->log(__VA_ARGS__)

#endif