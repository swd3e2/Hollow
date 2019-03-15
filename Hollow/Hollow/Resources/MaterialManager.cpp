#include "MaterialManager.h"

void MaterialManager::StartUp()
{
	setStartedUp();
}

void MaterialManager::Shutdown()
{
	setShutdown();
}

void MaterialManager::createMaterial(std::string name)
{
}

std::unordered_map<std::string, D3DMaterial*>* MaterialManager::getMaterialList()
{
	return &materialList;
}
