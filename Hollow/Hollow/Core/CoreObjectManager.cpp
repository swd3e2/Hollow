#include "CoreObjectManager.h"
#include "CoreObject.h"

CoreObjectManager::CoreObjectManager() :
	objectCounter(0)
{}

void CoreObjectManager::registerObject(CoreObject* object)
{
	coreObjects[object->objectId] = object;
}

void CoreObjectManager::unregisterObject(CoreObject* object)
{
	coreObjects[object->objectId] = nullptr;
}

size_t CoreObjectManager::getNextObjectId()
{
	return objectCounter++;
}
