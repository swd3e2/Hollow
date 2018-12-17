#pragma once
#include "Hollow/Memory/MemoryChunkManager.h"
#include "Hollow/Utils/FamilyTypeID.h"
#include "Hollow/Common/Log.h"
#include "Hollow/Platform.h"
#include "Components/IComponent.h"
#include "EntityManager.h"
#include <unordered_map>
#include <assert.h>
#include <limits.h>
#undef max
#undef min
#define ENITY_LUT_GROW 512
#define COMPONENT_LUT_GROW 512

namespace Hollow {

	using ComponentTypeID = size_t;
	using ComponentId = size_t;
	using EntityID = size_t;

	static const size_t INVALID_OBJECT_ID = std::numeric_limits<size_t>::max();

	class HOLLOW_API ComponentManager
	{
	private:
		class IComponentContainer
		{
		public:
			virtual ~IComponentContainer() {}
			virtual const char* GetComponentContainerTypeName() const = 0;
			virtual void DestroyComponent(IComponent* object) = 0;
			virtual size_t GetContainerMemoryUsed() = 0;
		};

		template<class T>
		class ComponentContainer : public Core::Memory::MemoryChunkManager<T, 512>, public IComponentContainer
		{
			ComponentContainer(const ComponentContainer&) = delete;
			ComponentContainer& operator=(ComponentContainer&) = delete;
		public:
			ComponentContainer() : Core::Memory::MemoryChunkManager<T, 512>("ComponentManager") {}

			virtual ~ComponentContainer() {}

			virtual const char* GetComponentContainerTypeName() const override
			{
				static const char* COMPONENT_TYPE_NAME{ typeid(T).name() };
				return COMPONENT_TYPE_NAME;
			}

			virtual void DestroyComponent(IComponent* object) override
			{
				object->~IComponent();
				this->DestroyObject(object);
			}

			virtual size_t GetContainerMemoryUsed() override { return this->GetMemoryUsed(); };
		};

		std::unordered_map<ComponentTypeID, IComponentContainer*> m_ComponentContainerRegistry;

		template<class T>
		inline ComponentContainer<T>* GetComponentContainer()
		{
			ComponentTypeID CID = T::STATIC_COMPONENT_TYPE_ID;

			auto it = this->m_ComponentContainerRegistry.find(CID);
			ComponentContainer<T>* cc = nullptr;

			if (it == this->m_ComponentContainerRegistry.end())
			{
				cc = new ComponentContainer<T>();
				this->m_ComponentContainerRegistry[CID] = cc;
			}
			else
				cc = static_cast<ComponentContainer<T>*>(it->second);

			assert(cc != nullptr && "Failed to create ComponentContainer<T>!");
			return cc;
		}

		std::vector<IComponent*> m_ComponentLUT;
		std::vector<std::vector<ComponentId>> m_EntityComponentMap;

		ComponentId	AqcuireComponentId(IComponent* component);
		void		ReleaseComponentId(ComponentId id);

		void		MapEntityComponent(EntityID entityId, ComponentId componentId, ComponentTypeID componentTypeId);
		void		UnmapEntityComponent(EntityID entityId, ComponentId componentId, ComponentTypeID componentTypeId);
	public:
		template<class T>
		using TComponentIterator = typename ComponentContainer<T>::iterator;

		ComponentManager();
		~ComponentManager();

		template<class T, class ...ARGS>
		T* AddComponent(const EntityID entityId, ARGS&&... args)
		{
			// hash operator for hashing entity and component ids
			static constexpr std::hash<ComponentId> ENTITY_COMPONENT_ID_HASHER{ std::hash<ComponentId>() };

			const ComponentTypeID CTID = T::STATIC_COMPONENT_TYPE_ID;

			// aqcuire memory for new component object of type T
			void* pObjectMemory = GetComponentContainer<T>()->CreateObject();

			ComponentId componentId = this->AqcuireComponentId((T*)pObjectMemory);

			// create component inplace
			IComponent* component = new (pObjectMemory)T(std::forward<ARGS>(args)...);
			((T*)pObjectMemory)->ComponentID = componentId;

			component->EntityID = entityId;
			component->HashValue = ENTITY_COMPONENT_ID_HASHER(entityId) ^ (ENTITY_COMPONENT_ID_HASHER(componentId) << 1);

			// create mapping from entity id its component id
			MapEntityComponent(entityId, componentId, CTID);
			//Hollow::Log::GetCoreLogger()->info("ComponentManager: created component with id {}, typeID {}, pointer {}", componentId, T::STATIC_COMPONENT_TYPE_ID, pObjectMemory);

			return (T*)(component);
		}

		template<class T>
		void RemoveComponent(const EntityID entityId)
		{
			const ComponentTypeID CTID = T::STATIC_COMPONENT_TYPE_ID;

			const ComponentId componentId = this->m_EntityComponentMap[entityId.index][CTID];

			IComponent* component = this->m_ComponentLUT[componentId];

			assert(component != nullptr && "FATAL: Trying to remove a component which is not used by this entity!");

			// release object memory
			GetComponentContainer<T>()->DestroyObject(component);

			// unmap entity id to component id
			UnmapEntityComponent(entityId, componentId, CTID);
		}

		void RemoveAllComponents(const EntityID entityId)
		{
			static const size_t NUM_COMPONENTS = this->m_EntityComponentMap[0].size();

			for (ComponentTypeID componentTypeId = 0; componentTypeId < NUM_COMPONENTS; ++componentTypeId)
			{
				const ComponentId componentId = this->m_EntityComponentMap[entityId][componentTypeId];
				if (componentId == INVALID_OBJECT_ID)
					continue;

				IComponent* component = this->m_ComponentLUT[componentId];
				if (component != nullptr)
				{
					// get appropriate component container
					auto it = this->m_ComponentContainerRegistry.find(componentTypeId);
					if (it != this->m_ComponentContainerRegistry.end())
						it->second->DestroyComponent(component);
					else
						assert(false && "Trying to release a component that wasn't created by ComponentManager!");

					// unmap entity id to component id
					UnmapEntityComponent(entityId, componentId, componentTypeId);
				}
			}
		}

		template<class T>
		T* GetComponent(const EntityID entityId)
		{
			const ComponentTypeID CTID = T::STATIC_COMPONENT_TYPE_ID;

			const ComponentId componentId = this->m_EntityComponentMap[entityId][CTID];

			// entity has no component of type T
			if (componentId == INVALID_OBJECT_ID)
				return nullptr;

			return static_cast<T*>(this->m_ComponentLUT[componentId]);
		}

		template<class T>
		inline TComponentIterator<T> begin()
		{
			return GetComponentContainer<T>()->begin();
		}

		template<class T>
		inline TComponentIterator<T> end()
		{
			return GetComponentContainer<T>()->end();
		}

		std::vector<size_t> GetMemoryUsed()
		{
			std::vector<size_t> memoryUsed;
			for (auto it : this->m_ComponentContainerRegistry)
				memoryUsed.push_back(it.second->GetContainerMemoryUsed());
			return memoryUsed;
		}
	};

}