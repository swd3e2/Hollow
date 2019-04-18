#pragma once


#ifndef CORE_OBJECT_H
#define CORE_OBJECT_H
#include "CoreObjectManager.h"
#include "ICoreObject.h"

class CoreObject : public ICoreObject
{
	friend class CoreObjectManager;
private:
	size_t objectId;
public:
	CoreObject();
	void initialize();
	void destroy();
};

#endif