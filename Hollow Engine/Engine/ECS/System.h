#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "ISystem.h"
#include "Utils/FamilyTypeID.h"

namespace ECS {

	template<class T>
	class System : public ISystem
	{
		friend class SystemManager;
	private:
		SystemManager* m_SystemManagerInstance;
	protected:
	public:
		static const SystemTypeId STATIC_SYSTEM_TYPE_ID;
	protected:
		System() { }
	public:
		virtual ~System()
		{
		}

		virtual inline const SystemTypeId GetStaticSystemTypeID() const
		{
			return STATIC_SYSTEM_TYPE_ID;
		}

		virtual inline const char* GetSystemTypeName() const override
		{
			static const char* SYSTEM_TYPE_NAME{ typeid(T).name() };
			return SYSTEM_TYPE_NAME;
		}

		// Adds a new dependencies for this system.
		template<class... Dependencies>
		void AddDependencies(Dependencies&&... dependencies)
		{
			this->m_SystemManagerInstance->AddSystemDependency(this, std::forward<Dependencies>(dependencies)...);
		}

		virtual void PreUpdate(f32 dt) override
		{}

		virtual void Update(f32 dt) override
		{}

		virtual void PostUpdate(f32 dt) override
		{}

	};

	template<class T>
	const SystemTypeId System<T>::STATIC_SYSTEM_TYPE_ID = util::Internal::FamilyTypeID<ISystem>::Get<T>();

}

#endif 