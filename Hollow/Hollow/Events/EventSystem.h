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
#include <mutex>

namespace Hollow {
	class EventSystem : public CModule<EventSystem>
	{
	private:
		std::unordered_map<int, std::vector<IEventDelegate*>> eventListeners;
		std::vector<IEvent*> events;
		std::mutex addMutex;
	public:
		template<class T>
		void addEventListener(IEventListener* listener, void (T::* func)(IEvent*), eventId id)
		{
			eventListeners[id].push_back(new EventDelegate<T>(listener, func, id));
		}

		/*
		 * todo: make event creation with move semantic
		 */
		void addEvent(IEvent* event)
		{
			addMutex.lock();
			events.push_back(event);
			addMutex.unlock();
		}

		void dispatch()
		{
			addMutex.lock();
			for (auto& ev : events) {
				const int eventId = ev->getId();
				if (eventListeners.find(eventId) != eventListeners.end()) {
					for (auto& it : eventListeners[eventId]) {
						it->invoke(ev);
					}
				}
				delete ev;
			}
			events.clear();
			addMutex.unlock();
		}
	};
}

#endif