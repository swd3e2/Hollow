#pragma once

#ifndef HW_TIMER_H
#define HW_TIMER_H

#include <chrono>
#include "Hollow/Platform.h"

namespace Hollow {
	class Timer
	{
	public:
		Timer();
		double getMilisecondsElapsed();
		void restart();
		bool stop();
		bool start();
	private:
		bool isrunning = false;

		std::chrono::time_point<std::chrono::steady_clock> m_Start;
		std::chrono::time_point<std::chrono::steady_clock> m_Stop;
	};
}

#endif