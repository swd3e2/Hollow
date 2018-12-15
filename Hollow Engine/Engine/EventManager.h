#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__
#pragma once
#include "Events/IEvent.h"
#include <list>
#include <unordered_map>

namespace Hollow {

	class IEventDelegate;

	class EventManager
	{
	private:
		std::unordered_map<IEvent*, IEventDelegate*> m_EventDispatchMap;
	public:
		template<class T, class ...ARGS>
		inline void AddEvent(ARGS ...args)
		{
			size_t ETID = T::GetEventTypeID();

		}
	};

}

#endif
