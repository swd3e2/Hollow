#pragma once
#include "Hollow/Platform.h"

namespace Hollow {

	class EntityManager;
	class ComponentManager;
	using EntityTypeID = size_t;
	using EntityID = size_t;

	class HOLLOW_API IEntity
	{
		friend class EntityManager;
	private:
		ComponentManager * componentManager;
	protected:
		EntityID		m_EntityID;
		bool			m_Active;
	public:
		IEntity() : m_Active(true) {}

		virtual ~IEntity() {}

		template<class T>
		T* GetComponent() const
		{
			return this->componentManager->GetComponent<T>(this->m_EntityID);
		}

		template<class T, class ...P>
		T* AddComponent(P&&... param)
		{
			return this->componentManager->AddComponent<T>(this->m_EntityID, std::forward<P>(param)...);
		}

		template<class T>
		void RemoveComponent()
		{
			this->componentManager->RemoveComponent<T>(this->m_EntityID);
		}

		// COMPARE ENTITIES

		inline bool operator==(const IEntity& rhs) const { return this->m_EntityID == rhs.m_EntityID; }
		inline bool operator!=(const IEntity& rhs) const { return this->m_EntityID != rhs.m_EntityID; }
		inline bool operator==(const IEntity* rhs) const { return this->m_EntityID == rhs->m_EntityID; }
		inline bool operator!=(const IEntity* rhs) const { return this->m_EntityID != rhs->m_EntityID; }

		// ACCESORS
		virtual const EntityTypeID GetStaticEntityTypeID() const = 0;

		inline const EntityID GetEntityID() const { return this->m_EntityID; }

		inline void SetActive(bool active) 
		{
			if (this->m_Active == active)
				return;

			if (active == false)
			{
				this->OnDisable();
			}
			else
			{
				this->OnEnable();
			}

			this->m_Active = active;
		}

		inline bool IsActive() const { return this->m_Active; }

		virtual void OnEnable() {}
		virtual void OnDisable() {}
	};

}