#ifndef __SYSTEM_MANAGER_H__
#define __SYSTEM_MANAGER_H__
#include "Systems/ISystem.h"
#include <list>

namespace Hollow {

	class SystemManager
	{
	private:
		std::list<ISystem*> m_Systems;
	public:
		template<class S>
		void AddSystem(S* system)
		{
			this->m_Systems.push_back(system);
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

		void UpdateSystems()
		{

		}
	};

}

#endif