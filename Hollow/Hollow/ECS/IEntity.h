#pragma once

#ifndef HW_I_ENTITY_H
#define HW_I_ENTITY_H

#include <utility>
#include "ComponentManager.h"

class IEntity
{
	friend class ComponentManager;
protected:
	size_t entityId;
public:
	size_t getId()
	{
		return entityId;
	}

	template<class T, class ...ARGS>
	T* addComponent(ARGS&& ... args)
	{
		return ComponentManager::instance()->create<T>(entityId, std::forward<ARGS>(args)...);
	}

	template<class T>
	void destroyComponent()
	{
		ComponentManager::instance()->destroy<T>(entityId);
	}

	template<class T>
	T* getComponent()
	{
		return ComponentManager::instance()->get<T>(entityId);
	}

	template<class T>
	bool hasComponent()
	{
		return ComponentManager::instance()->has<T>(entityId);
	}
};

#endif