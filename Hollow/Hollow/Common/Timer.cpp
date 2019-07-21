#include "Timer.h"

namespace Hollow {
	Timer::Timer()
	{
		m_Start = std::chrono::high_resolution_clock::now();
		m_Stop = std::chrono::high_resolution_clock::now();
	}

	double Timer::getMilisecondsElapsed()
	{
		if (isrunning)
		{
			auto elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - m_Start);
			return elapsed.count();
		}
		else
		{
			auto elapsed = std::chrono::duration<double, std::milli>(m_Stop - m_Start);
			return elapsed.count();
		}
	}

	void Timer::restart()
	{
		isrunning = true;
		m_Start = std::chrono::high_resolution_clock::now();
	}

	bool Timer::stop()
	{
		if (!isrunning)
			return false;
		else
		{
			m_Stop = std::chrono::high_resolution_clock::now();
			isrunning = false;
			return true;
		}
	}

	bool Timer::start()
	{
		if (isrunning)
		{
			return false;
		}
		else
		{
			m_Start = std::chrono::high_resolution_clock::now();
			isrunning = true;
			return true;
		}
	}
}