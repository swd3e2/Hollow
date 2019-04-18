#pragma once

#ifndef HW_COMPONENT_MANAGER_H
#define HW_COMPONENT_MANAGER_H

#include <unordered_map>

#include "Hollow/Core/CModule.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Containers/array.h"
#include "Hollow/Platform.h"
#include "IEntity.h"
#include "Component.h"

class HOLLOW_API ComponentManager : public CModule<ComponentManager>
{
private:
	class IComponentContainer {};

	template<class T>
	class ComponentContainer : public IComponentContainer
	{
	public:
		Hollow::array<T> componentList;
	};

	std::unordered_map<size_t, IComponentContainer*> componentContainers;
	/* +ComponentMap---------------------------------+
	   |EntityId    |ComponentId |MeshComponent      |
	   |            |--------------------------------|
	   |            |ComponentId |CollisionComponent |
	   |            |--------------------------------|
	   |            |ComponentId |TransformComponent |
	   |---------------------------------------------|
	   |EntityId    |ComponentId |TransformComponent |
	   |            |--------------------------------|
	   |            |ComponentId |LightComponent     |
	   +---------------------------------------------+ */
	std::unordered_map<size_t, std::unordered_map<size_t, IComponent*>> componentMap;
public:
	template<class T>
	ComponentContainer<T>* getContainer()
	{
		size_t componentTypeId = T::staticGetTypeId();

		// Trying to find container, if found - just return it
		if (componentContainers.find(componentTypeId) != componentContainers.end())
		{
			return (ComponentContainer<T>*)(componentContainers[componentTypeId]);
		}
		// if not - need to create one
		ComponentContainer<T>* container = new ComponentContainer<T>();
		componentContainers[componentTypeId] = container;

		return container;
	}

	template<class T, typename ...ARGS>
	T* create(IEntity* entity, ARGS&& ... args) {
		size_t componentTypeId = T::staticGetTypeId();
		// todo: maybe no need in delete component if trying to add one
		destroy<T>(entity);

		ComponentContainer<T>* container = getContainer<T>();
		T* component = container->componentList.createObject(std::move(args)...);
		componentMap[entity->entityId][componentTypeId] = component;

		return component;
	}

	template<class T>
	void destroy(IEntity* entity)
	{
		if (has<T>(entity))
		{
			size_t componentTypeId = T::staticGetTypeId();

			ComponentContainer<T>* container = getContainer<T>();
			container->componentList.destroyObject((T*)componentMap[entity->entityId][componentTypeId]);
		}
	}

	template<class T>
	bool has(IEntity* entity)
	{
		size_t componentTypeId = T::staticGetTypeId();

		if (componentMap.find(entity->entityId) != componentMap.end())
		{
			if (componentMap[entity->entityId].find(componentTypeId) != componentMap[entity->entityId].end())
			{
				return true;
			}
		}
		return false;
	}

	template<class T>
	T* get(IEntity* entity)
	{
		if (has<T>(entity))
		{
			size_t componentTypeId = T::staticGetTypeId();

			ComponentContainer<T>* container = getContainer<T>();
			return (T*)componentMap[entity->entityId][componentTypeId];
		}

		return nullptr;
	}

	void startUp() { setStartedUp(); }
	void shutdown() { setShutdown(); }
};


#endif