#ifndef CORE_OBJECT_MANAGER_H
#define CORE_OBJECT_MANAGER_H
#pragma once
#include <map>
#include "Hollow/Core/CModule.h"
#include "ICoreObject.h"

class CoreObjectManager : public CModule<CoreObjectManager>
{
private:
	size_t objectCounter;
	std::map<CoreObject*, int> coreObjects;
public:
	CoreObjectManager();

	void registerObject(CoreObject* object);
	void unregisterObject(CoreObject* object);
	size_t getNextObjectId();
};

#endif