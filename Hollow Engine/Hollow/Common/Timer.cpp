#include "Timer.h"

namespace Hollow {
	Timer::Timer() :
		m_Elapsed(0)
	{}

	Timer::~Timer()
	{}

	void Timer::Tick(float_t ms)
	{
		this->m_Elapsed += std::chrono::duration<float, std::ratio<1, 1000>>(ms);
	}

	void Timer::Reset()
	{
		this->m_Elapsed = Elapsed::zero();
	}
}
