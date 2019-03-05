#pragma once
#include "Hollow/Memory/MemoryChunkManager.h"
#include "Entities/IEntity.h"
#include "Hollow/Platform.h"
#include "Hollow/Containers/vector.h"
#include "Hollow/Containers/pair.h"

#include "ComponentManager.h"
#include <unordered_map>
#include <new>
#include <vector>

#define ENTTIY_TABLE_GROW 1024

namespace Hollow {

	// Entity manager
	class HOLLOW_API EntityManager
	{
	private:
		ComponentManager * m_ComponentManager;

		class HOLLOW_API IEntityContainer
		{
		public:
			virtual ~IEntityContainer() {}
			virtual const char* GetEntityContainerTypeName() const = 0;
			virtual void DestroyEntity(IEntity* object) = 0;
			virtual size_t GetContainerMemoryUsed() = 0;
		};

		template<class E>
		class EntityContainer : public Core::Memory::MemoryChunkManager<E, 256>, public IEntityContainer
		{
			EntityContainer(const EntityContainer&) = delete;
			EntityContainer& operator=(EntityContainer&) = delete;
		public:
			EntityContainer() : Core::Memory::MemoryChunkManager<E, 256>("EntityManager") {}

			virtual ~EntityContainer() {}

			virtual const char* GetEntityContainerTypeName() const override
			{
				static const char* ENTITY_TYPE_NAME{ typeid(E).name() };
				return ENTITY_TYPE_NAME;
			}

			virtual void DestroyEntity(IEntity* object) override
			{
				object->~IEntity();
				this->DestroyObject(object);
			}
			virtual size_t GetContainerMemoryUsed() override { return this->GetMemoryUsed(); };
		};

		template<class E>
		inline EntityContainer<E>* GetEntityContainer()
		{
			EntityTypeID EID = E::STATIC_ENTITY_TYPE_ID;
			auto it = this->m_EntityRegistry.find(EID);
			EntityContainer<E>* ec = nullptr;
			if (it == this->m_EntityRegistry.end())
			{
				ec = new EntityContainer<E>();
				this->m_EntityRegistry[EID] = ec;
			}
			else
				ec = (EntityContainer<E>*)it->second;

			assert(ec != nullptr && "Failed to create EntityContainer<T>!");
			return ec;
		}

	private:
		static EntityManager* _instance;
		std::unordered_map<EntityTypeID, IEntityContainer*> m_EntityRegistry;
		//std::vector<std::pair<EntityID, void*>> entityTable;
		Containers::Vector<Containers::Pair<EntityID, void*>, 1024> entityTable;
		size_t HandleEntityCount;
	public:

		EntityManager(ComponentManager * componentManager)
			: m_ComponentManager(componentManager)
		{
			HW_DEBUG("EntityManager: created");
			this->HandleEntityCount = 0;
			
			if (_instance == nullptr)
				_instance = this;

			for (int i = 0; i < ENTTIY_TABLE_GROW; i++)
				this->entityTable.push_back(Containers::Pair<EntityID, void*>( i , nullptr ));
		}

		// Get entity id
		size_t AcquireEntityID(IEntity* entity)
		{
			if (this->HandleEntityCount + 1 > entityTable.size()) {
				UINT iteration = (UINT)(this->HandleEntityCount / ENTTIY_TABLE_GROW);
				for (int i = 0; i < ENTTIY_TABLE_GROW; i++)
					this->entityTable.push_back(Containers::Pair<EntityID, void*>( i + iteration * ENTTIY_TABLE_GROW , nullptr ));
			}
			for (int i = 0; i < this->entityTable.size(); i++)
			{
				if (this->entityTable[i].second == nullptr)
				{
					this->entityTable[i].second = (void*)entity;
					this->HandleEntityCount++;
					return this->entityTable[i].first;
				}
			}
			return 0;
		}

		template<class E, class ...ARGS>
		E* CreateEntity(ARGS&&... args)
		{
			EntityContainer<E>* container = this->GetEntityContainer<E>();
			void* entityMemory = container->CreateObject();

			IEntity* entity = new (entityMemory)E(std::forward<ARGS>(args)...);

			size_t id = this->AcquireEntityID((E*)entityMemory);

			((E*)entityMemory)->m_EntityID = id;
			((E*)entityMemory)->componentManager = this->m_ComponentManager;

			return (E*)entityMemory;
		}

		template<class E>
		void DestroyEntity(EntityID entityId)
		{
			EntityContainer<E>* container = this->GetEntityContainer<E>();
			IEntity* entityMemory = nullptr;

			for (int i = 0; i < this->entityTable.size(); i++)
			{
				if (this->entityTable[i].first == entityId)
				{
					entityMemory = (IEntity*)this->entityTable[i].second;
					HW_INFO("EntityManager: destroyed entity with id {}, typeID {}, pointer {}", entityMemory->GetEntityID(), E::STATIC_ENTITY_TYPE_ID, (void*)entityMemory);

					this->m_ComponentManager->RemoveAllComponents(entityMemory->GetEntityID());
					container->DestroyEntity(entityMemory);
					this->entityTable[i] = {entityId, nullptr};
					this->HandleEntityCount--;
					break;
				}
			}
		}

		std::vector<size_t> GetMemoryUsed()
		{ 
			std::vector<size_t> memoryUsed;
			for (auto it : this->m_EntityRegistry)
				memoryUsed.push_back(it.second->GetContainerMemoryUsed());
			return memoryUsed;
		}

		Containers::Vector<IEntity*>* GetEntitiesList()
		{
			Containers::Vector<IEntity*>* container = new Containers::Vector<IEntity*>;
			for (int i = 0; i < HandleEntityCount; i++)
				container->push_back((IEntity*)this->entityTable[i].second);

			return container;
		}

		IEntity* find(EntityID entityId)
		{
			IEntity* entity = nullptr;
			if (HandleEntityCount > entityId)
			{
				entity = (IEntity*)this->entityTable[entityId].second;
			}

			return entity;
		}

		static EntityManager* instance()
		{
			return _instance;
		}
	};

}