#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
	setStartedUp();
}

ShaderManager::~ShaderManager()
{
	setShutdown();
}
