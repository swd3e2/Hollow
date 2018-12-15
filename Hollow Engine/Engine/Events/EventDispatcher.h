#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__
#pragma once
#include "IEventDispatcher.h"
#include "IEventDelegate.h"

namespace Hollow {

	template<class EventType>
	class EventDispatcher : public IEventDispatcher
	{
	private:
		std::list<IEventDelegate*>	m_EventDeleagteList;
		std::list<IEventDelegate*>	m_PendingRemoveDelegates;
		bool						m_Locked;
	public:
		EventDispatcher() : m_Locked(false) {}

		virtual ~EventDispatcher()
		{
			m_EventDeleagteList.clear();
			m_PendingRemoveDelegates.clear();
		}
 
		virtual void Dispatch(IEvent* event) override
		{
			this->m_Locked = true;
			{
				if (this->m_PendingRemove)
			}
			this->m_Locked = false;
		}

		virtual void AddEventCallback(IEventDelegate* const eventDelegate) = 0;

		virtual void RemoveEventCallback(IEventDelegate* eventDelegate) = 0;

		virtual inline size_t GetEventCallbackCount() const = 0;
	};

}

#endif