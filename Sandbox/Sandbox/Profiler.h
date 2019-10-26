#pragma once

#include <unordered_map>
#include <assert.h>
#include "Hollow/Common/Log.h"
#include "Hollow/Common/Timer.h"

class Profiler
{
private:
	Hollow::Timer timer;
	std::unordered_map<std::string, double> values;
	bool isProfilerActive = false;
	std::string currentProfilingName;

	static Profiler* _instance;
public:
	Profiler()
	{
		if (_instance == nullptr) {
			_instance = this;
		}
	}

	static void begin(const std::string& name)
	{
		if (_instance->isProfilerActive) {
			HW_ERROR("Mismatching begin|end");
			assert(false);
		}
		
		_instance->currentProfilingName = name;
		_instance->isProfilerActive = true;
		_instance->timer.restart();
	}

	static void end()
	{
		if (!_instance->isProfilerActive) {
			HW_ERROR("Mismatching begin|end");
			assert(false);
		}

		_instance->isProfilerActive = false;
		_instance->timer.stop();
		_instance->values[_instance->currentProfilingName] = _instance->timer.getMilisecondsElapsed();
	}

	static inline const std::unordered_map<std::string, double>& getValues() { return _instance->values; }
};