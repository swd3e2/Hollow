#pragma once
#include "../Platform.h"
#include "Utils/Handle.h"

namespace ECS {

	using EntityTypeId = TypeID;
	using EntityId = util::Handle64;
	static const EntityId INVALID_ENTITY_ID = util::Handle64::INVALID_HANDLE;

	class IEntity
	{
		friend class EntityManager;
	private:
		// set on create; in EntityManager
		//ComponentManager*	m_ComponentManagerInstance;
	protected:
		// set on create; in EntityManager
		EntityId				m_EntityID;
		// if false, entity won't be updated
		bool					m_Active;
	public:
		IEntity();
		virtual ~IEntity();

		template<class T>
		T* GetComponent() const
		{
			//return this->m_ComponentManagerInstance->GetComponent<T>(this->m_EntityID);
		}

		template<class T, class ...P>
		T* AddComponent(P&&... param)
		{
			//return this->m_ComponentManagerInstance->AddComponent<T>(this->m_EntityID, std::forward<P>(param)...);
		}

		template<class T>
		void RemoveComponent()
		{
			//this->m_ComponentManagerInstance->RemoveComponent<T>(this->m_EntityID);
		}

		// COMPARE ENTITIES

		inline bool operator==(const IEntity& rhs) const { return this->m_EntityID == rhs.m_EntityID; }
		inline bool operator!=(const IEntity& rhs) const { return this->m_EntityID != rhs.m_EntityID; }
		inline bool operator==(const IEntity* rhs) const { return this->m_EntityID == rhs->m_EntityID; }
		inline bool operator!=(const IEntity* rhs) const { return this->m_EntityID != rhs->m_EntityID; }

		// ACCESORS
		virtual const EntityTypeId GetStaticEntityTypeID() const = 0;

		inline const EntityId GetEntityID() const { return this->m_EntityID; }

		void SetActive(bool active);

		inline bool IsActive() const { return this->m_Active; }

		virtual void OnEnable() {}
		virtual void OnDisable() {}
	};

}