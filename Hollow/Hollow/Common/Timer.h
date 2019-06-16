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
		double GetMilisecondsElapsed();
		void Restart();
		bool Stop();
		bool Start();
	private:
		bool isrunning = false;

		std::chrono::time_point<std::chrono::steady_clock> start;
		std::chrono::time_point<std::chrono::steady_clock> stop;
	};
}

#endif