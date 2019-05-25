#pragma once

#ifndef HW_SYSTEM_H
#define HW_SYSTEM_H

#include "Hollow/Platform.h"
#include "ISystem.h"
#include <typeinfo>

namespace Hollow {

	template<class T>
	class System : public ISystem
	{
	public:
		static const SystemTypeID STATIC_SYSTEM_TYPE_ID;
	private:
		//SystemManager* m_SystemManagerInstance;
	protected:
		System()
		{}
	public:
		virtual ~System()
		{}
		
		virtual inline const SystemTypeID GetStaticSystemTypeID() const { return STATIC_SYSTEM_TYPE_ID; }
		
		virtual inline const char* GetSystemTypeName() const override
		{
			static const char* SYSTEM_TYPE_NAME{ typeid(T).name() };
			return SYSTEM_TYPE_NAME;
		}

		template<class... Dependencies>
		void AddDependencies(Dependencies&&... dependencies)
		{
			//this->m_SystemManagerInstance->AddSystemDependency(this, std::forward<Dependencies>(dependencies)...);
		}

		virtual void PreUpdate(double dt) override
		{}

		virtual void Update(double dt) override
		{}

		virtual void PostUpdate(double dt) override
		{}
	};

	template<class T>
	const SystemTypeID System<T>::STATIC_SYSTEM_TYPE_ID = typeid(T).hash_code();
}

#endif