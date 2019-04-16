#pragma once

#ifndef HW_I_ENTITY_H
#define HW_I_ENTITY_H
#include <utility>

class ComponentManager;
class IEntity
{
	friend class ComponentManager;
protected:
	size_t entityId;
public:
	template<class T, class ...ARGS>
	void addComponent(ARGS&& ...args)
	{
		ComponentManager::instance()->create(this, std::move(args)...);
	}

	template<class T>
	void destroyComponent()
	{
		ComponentManager::instance()->destroy<T>(this);
	}

	template<class T>
	void hasComponent()
	{
		ComponentManager::instance()->has<T>(this);
	}
};

#endif