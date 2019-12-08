#include "Random.h"

namespace Hollow {
	std::mt19937 Random::generator { std::random_device()() };
}