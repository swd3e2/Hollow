#include "Log.h"

namespace Hollow {
	std::shared_ptr<spdlog::logger> Log::m_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::m_CoreLogger;
}