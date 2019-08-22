#include "InputManager.h"

namespace Hollow {
	bool InputManager::MouseButton[5];
	bool InputManager::KeyboardKeys[256];
	long InputManager::mx = 0;
	long InputManager::my = 0;
	long InputManager::lastX = 0;
	long InputManager::lastY = 0;

	float InputManager::mcx = 0;
	float InputManager::mcy = 0;
}