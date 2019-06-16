#pragma once

#ifndef HW_EVENT_SYSTEM_H
#define HW_EVENT_SYSTEM_H

#include <vector>
#include "IEvent.h"
#include "IEventListener.h"
#include <unordered_map>
#include "EventDelegate.h"
#include <vector>
#include "Hollow/Core/CModule.h"

namespace Hollow {
	class EventSystem : public CModule<EventSystem>
	{
	private:
		std::vector<IEventDelegate*>  eventListeners;
		std::vector<IEvent*> events;
	public:
		void startUp()
		{
			setStartedUp();
		}

		void shutdown()
		{
			setShutdown();
		}

		template<class T>
		void addEventListener(IEventListener* listener, void (T::* func)(IEvent*), eventId id)
		{
			eventListeners.push_back((IEventDelegate*)new EventDelegate<T>(listener, func, id));
		}

		void addEventListener(IEventDelegate* delegate)
		{
			eventListeners.push_back(delegate);
		}

		/*
		 * todo: make event creation with move semantic
		 */
		void addEvent(IEvent* event)
		{
			events.push_back(event);
		}

		void dispatch()
		{
			for (auto& ev : events) {
				for (auto& it : eventListeners) {
					if (it != nullptr)
						if (ev->getEventId() == it->getSubscribedEventId()) {
							it->invoke(ev);
						}
				}
				delete ev;
			}
			events.clear();
		}
	};
}

#endif