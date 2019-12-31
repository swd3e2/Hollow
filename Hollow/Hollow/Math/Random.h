#pragma once

#include <random>

namespace Hollow {
	class Random
	{
	private:
		static std::mt19937 generator;
	public:
		static int randomInt(int from, int to)
		{
			std::uniform_int_distribution<int> uid(from, to);
			return uid(generator);
		}

		static float randomFloat(float from, float to)
		{
			std::uniform_real_distribution<float> uid(from, to);
			return uid(generator);
		}
	};
}