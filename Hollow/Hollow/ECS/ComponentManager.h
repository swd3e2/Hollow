#pragma once

#ifndef HW_COMPONENT_MANAGER_H
#define HW_COMPONENT_MANAGER_H

#include <unordered_map>

#include "Hollow/Core/CModule.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Platform.h"
#include "Component.h"
#include "Hollow/Memory/MemoryContainer.h"

namespace Hollow {
	class ComponentManager : public CModule<ComponentManager>
	{
	private:
		class IComponentContainer {
		public:
			virtual ~IComponentContainer() {}

			virtual void destroy(void* mem) = 0;
		};

		template<class T>
		class ComponentContainer : public IComponentContainer
		{
		public:
			MemoryContainer<T> container;
		public:
			ComponentContainer() {}
			virtual ~ComponentContainer() {}

			virtual void destroy(void* mem) override
			{
				container.deallocate(mem);
			}
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
			if (componentContainers.find(componentTypeId) != componentContainers.end()) {
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
			T* component = new (container->container.allocate()) T(std::forward<ARGS>(args)...);
			componentMap[entityId][componentTypeId] = component;

			return component;
		}

		template<class T>
		void destroy(size_t entityId)
		{
			if (has<T>(entityId)) {
				size_t componentTypeId = T::staticGetTypeId();

				ComponentContainer<T>* container = getContainer<T>();
				container->container.deallocate((T*)componentMap[entityId][componentTypeId]);
			}
		}

		void destroyAll(size_t entityId)
		{
			if (componentMap.find(entityId) != componentMap.end())
			{
				for (auto& it : componentMap[entityId]) {
					size_t componentTypeId = it.second->getTypeId();
					componentContainers[componentTypeId]->destroy(it.second);
				}
				componentMap.erase(entityId);
			}
		}

		template<class T>
		bool has(size_t entityId)
		{
			size_t componentTypeId = T::staticGetTypeId();

			if (componentMap.find(entityId) != componentMap.end()) {
				if (componentMap[entityId].find(componentTypeId) != componentMap[entityId].end()) {
					return true;
				}
			}
			return false;
		}

		template<class T>
		T* get(size_t entityId)
		{
			if (has<T>(entityId)) {
				size_t componentTypeId = T::staticGetTypeId();

				ComponentContainer<T>* container = getContainer<T>();
				return (T*)componentMap[entityId][componentTypeId];
			}

			return nullptr;
		}

		template<class T>
		typename MemoryContainer<T>::iterator& begin()
		{
			return getContainer<T>()->container.begin();
		}

		template<class T>
		typename MemoryContainer<T>::iterator& end()
		{
			return getContainer<T>()->container.end();
		}
	};
}

#endif