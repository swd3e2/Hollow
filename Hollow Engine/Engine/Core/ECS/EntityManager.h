#pragma once
#include "Utils/Handle.h"
#include "IEntity.h"
#include "../Memory/MemoryChunkAllocator.h"
#include <unordered_map>

#define ENTITY_T_CHUNK_SIZE					512

namespace ECS {

	class EntityManager
	{
		using EntityHandleTable = util::HandleTable<IEntity, std::size_t>;

		class IEntityContainer
		{
			virtual ~IEntityContainer() {};

			virtual char * GetEntityContainerTypeName() const = 0;
			virtual void DestroyEntity(IEntity * entity) = 0;
		};

		template<class T>
		class EntityContainer : public Memory::MemoryChunkAllocator<T, ENTITY_T_CHUNK_SIZE>, public IEntityContainer
		{
			EntityContainer(const EntityContainer&) = delete;
			EntityContainer& operator=(EntityContainer&) = delete;
		public:
			EntityContainer() : MemoryChunkAllocator("EntityManager")
			{}

			virtual ~EntityContainer() {}

			virtual const * GetEntityContainerTypeName() const override
			{
				static const char* ENTITY_TYPE_NAME{typeid(T).name()};
				return ENTITY_TYPE_NAME;
			}

			virtual DestroyEntity(IEntity * entity) override
			{
				entity->~IEntity();
				this->DestroyObject(entity);
			}
		};

		std::unordered_map<std::size_t, IEntityContainer*>	m_EntityRegistry;
		std::vector<std::size_t>							m_PendingDestroyedEntites;
		std::size_t											m_NumPendingDestoyedEntities;

		//ComponentManager * componentManager;
	private:
		EntityManager(const EntityManager&) = delete;
		EntityManager& operator=(EntityManager&) = delete;

		EntityHandleTable m_EntityHandleTable;

		template<class T>
		inline EntityContainer<T>* GetEntityContainer()
		{
			std::size_t EID = T::STATIC_ENTITY_TYPE_ID;

			auto it = this->m_EntityRegistry.find(EID);
			EntityContainer<T>* ec = nullptr;
			if (it == this->m_EntityRegistry.end())
			{
				ec = new EntityContainer<T>();
				this->m_EntityRegistry[EID] = ec;
			}
			else
			{
				ec = (EntityContainer<T>*)it->second;
			}

			assert(ec != nullptr);
			return ec;
		}

		EntityId AqcuireEntityId(IEntity* entity);
		void ReleaseEntityId(EntityId id);
	public:
		EntityManager();
		//EntityManager(ComponentManager* componentManagerInstance);
		~EntityManager();

		template<class T, class ...ARGS>
		size_t CreateEntity(ARGS&& ...args)
		{
			void* pObjectMem = GetEntityContainer<T>();

			ECS::EntityId entityID = this->AqcuireEntityId((T*)pObjectMem);

			((T*)pObjectMem)->m_EntityID = entityID;
			((T*)pObjectMemory)->m_ComponentManagerInstance = this->m_ComponentManagerInstance;
				
			IEntity* entity = new (pObjcetMem)T(std::forward<ARGS>(args)...);
			return entityID;
		}

		void DestroyEntity(EntityId entityID)
		{
			IEntity* entity = this->m_EntityHandleTable[entityID];

			const EntityTypeId ETID = entity->GetStaticEntityTypeID();

			if (this->m_NumPendingDestoyedEntities < this->m_PendingDestroyedEntites.size())
			{
				this->m_PendingDestroyedEntites[this->m_NumPendingDestoyedEntities++] = entityID;
			}
			else
			{
				this->m_PendingDestroyedEntites.push_back(entityID);
				this->m_NumPendingDestoyedEntities++;
			}
		}
		
		inline IEntity* GetEntity(EntityId entityId)
		{
			return this->m_EntityHandleTable[entityId];
		}

		inline EntityId GetEntityId(EntityId::value_type index) const
		{
			return this->m_EntityHandleTable[index];
		}
	};

}