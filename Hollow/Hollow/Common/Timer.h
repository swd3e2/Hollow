#pragma once
#include <chrono>
#include <cstddef>
#include "Hollow/Platform.h"

namespace Hollow {

	class HOLLOW_API Timer
	{
		using Elapsed = std::chrono::duration<float_t, std::milli>;
	private:
		Elapsed m_Elapsed;

		union TimeStamp
		{
			float_t asFloat;
			unsigned int asUInt;

			TimeStamp() : asUInt(0U)
			{}

			TimeStamp(float_t floatValue) : asFloat(floatValue)
			{}

			operator unsigned int() const { return this->asUInt; }

			inline const bool operator==(const TimeStamp& other) const { return this->asUInt == other.asUInt; }
			inline const bool operator!=(const TimeStamp& other) const { return this->asUInt != other.asUInt; }

			inline const bool operator<(const TimeStamp& other) const { return this->asFloat < other.asFloat; }
			inline const bool operator>(const TimeStamp& other) const { return this->asFloat > other.asFloat; }
		};
	public:

		Timer();
		~Timer();

		void Tick(float_t ms);
		void Reset();

		inline TimeStamp GetTimeStamp() const
		{
			return TimeStamp(this->m_Elapsed.count());
		}

	};
}
