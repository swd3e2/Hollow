#pragma once
#include <chrono>
#include "Hollow/Platform.h"

class HOLLOW_API Timer
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