#include "InputManager.h"

namespace Hollow {
	bool InputManager::MouseButton[5];
	bool InputManager::KeyboardKeys[256];
	long InputManager::mx = 0;
	long InputManager::my = 0;
	
	float InputManager::mcx = 0;
	float InputManager::mcy = 0;

	float InputManager::pcx = 0;
	float InputManager::pcy = 0;

	float InputManager::pix = 0;
	float InputManager::piy = 0;
	float InputManager::piz = 0;

	std::vector<SimpleFieldVertex> InputManager::field;

	XMVECTOR InputManager::pickRayInWorldSpacePos;
	XMVECTOR InputManager::pickRayInWorldSpaceDir;
	XMVECTOR InputManager::pickRayInViewSpaceDir;
	XMVECTOR InputManager::pickRayInViewSpacePos;
}