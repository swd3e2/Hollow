#include "ShaderManager.h"

void Hollow::ShaderManager::onShutdown()
{
	for (auto& it : shaders) {
		delete it.second;
	}
}
