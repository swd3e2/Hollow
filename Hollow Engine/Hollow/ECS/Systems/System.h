#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include "Utils/FamilyTypeID.h"
#include "ISystem.h"
#include "Platform.h"

namespace Hollow {

	template<class T>
	class HOLLOW_API System : public ISystem
	{
	public:
		static const SystemTypeID STATIC_SYSTEM_TYPE_ID;
	private:
		//SystemManager* m_SystemManagerInstance;
	protected:
		System()
		{
		}
	public:
		virtual ~System()
		{
		}
		
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

		virtual void PreUpdate(float_t dt) override
		{}

		virtual void Update(float_t dt) override
		{}

		virtual void PostUpdate(float_t dt) override
		{}
	};

}

#endif