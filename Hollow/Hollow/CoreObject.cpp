#include "CoreObject.h"

CoreObject::CoreObject() :
	objectId(CoreObjectManager::instance()->getNextObjectId())
{
}

void CoreObject::initialize()
{
	CoreObjectManager::instance()->registerObject(this);
}

void CoreObject::destroy()
{
	CoreObjectManager::instance()->unregisterObject(this);
}
