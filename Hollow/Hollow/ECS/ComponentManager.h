#pragma once

#ifndef HW_COMPONENT_MANAGER_H
#define HW_COMPONENT_MANAGER_H

#include <unordered_map>

#include "Hollow/Core/CModule.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Containers/array.h"
#include "Hollow/Platform.h"
#include "Component.h"

namespace Hollow {
	class ComponentManager : public CModule<ComponentManager>
	{
	private:
		class IComponentContainer {};

		template<class T>
		class ComponentContainer : public IComponentContainer
		{
		public:
			Hollow::array<T> componentList;
		public:
			ComponentContainer() :
				componentList(100)
			{}
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
		~ComponentManager()
		{
			clear();
		}

		void clear()
		{
			for (auto& componentContainer : componentContainers) {
				delete componentContainer.second;
			}
			componentContainers.clear();

			for (auto& map : componentMap) {
				map.second.clear();
			}
		}

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

		template<class T, typename ... ARGS>
		T* create(size_t entityId, ARGS&& ... args)
		{
			size_t componentTypeId = T::staticGetTypeId();
			// todo: maybe don't need delete component if trying to add one
			destroy<T>(entityId);

			ComponentContainer<T>* container = getContainer<T>();
			T* component = container->componentList.createObject(std::forward<ARGS>(args)...);
			componentMap[entityId][componentTypeId] = component;

			return component;
		}

		template<class T>
		void destroy(size_t entityId)
		{
			if (has<T>(entityId))
			{
				size_t componentTypeId = T::staticGetTypeId();

				ComponentContainer<T>* container = getContainer<T>();
				container->componentList.destroyObject((T*)componentMap[entityId][componentTypeId]);
			}
		}

		template<class T>
		bool has(size_t entityId)
		{
			size_t componentTypeId = T::staticGetTypeId();

			if (componentMap.find(entityId) != componentMap.end())
			{
				if (componentMap[entityId].find(componentTypeId) != componentMap[entityId].end())
				{
					return true;
				}
			}
			return false;
		}

		template<class T>
		T* get(size_t entityId)
		{
			if (has<T>(entityId))
			{
				size_t componentTypeId = T::staticGetTypeId();

				ComponentContainer<T>* container = getContainer<T>();
				return (T*)componentMap[entityId][componentTypeId];
			}

			return nullptr;
		}

		void startUp() { setStartedUp(); }
		void shutdown() { setShutdown(); }
	};
}

#endif