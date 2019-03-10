#pragma once
#include "ISystem.h"
#include <list>
#include "Hollow/Platform.h"

namespace Hollow {
	class HOLLOW_API SystemManager
	{
	private:
		std::list<ISystem*> m_Systems;
	public:
		template<class T>
		void AddSystem(T* system)
		{
			this->m_Systems.push_back((ISystem*)system);
		}

		template<class S>
		void RemoveSystem(S* system)
		{
			auto it = this->m_Systems.find(system);
			if (it != this->m_Systems.end())
			{
				this->m_Systems.remove(system);
			}
		}

		void PreUpdateSystems(float dt)
		{
			for (auto& it : m_Systems)
				it->PreUpdate(dt);
		}

		void PostUpdateSystems(float dt)
		{
			for (auto& it : m_Systems)
				it->PreUpdate(dt);
		}

		void UpdateSystems(float dt)
		{
			for (auto& it : m_Systems)
				it->Update(dt);
		}
	};

}