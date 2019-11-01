#pragma once

#ifndef HW_EVENT_SYSTEM_H
#define HW_EVENT_SYSTEM_H

#include <vector>
#include "IEvent.h"
#include "IEventListener.h"
#include <unordered_map>
#include "EventDelegate.h"
#include <list>
#include "Hollow/Core/CModule.h"
#include <mutex>
#include <queue>

namespace Hollow {
	class EventSystem : public CModule<EventSystem>
	{
	private:
		std::unordered_map<int, std::vector<IEventDelegate*>> eventListeners;
		std::list<IEvent*> events;
		std::vector<IEvent*> eventsToRemove;
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
		void addEvent(IEvent* event, double time = 0)
		{
			event->time = time;
			addMutex.lock();
			events.push_back(event);
			addMutex.unlock();
		}

		void dispatch(double dt)
		{
			addMutex.lock();
			for (auto& event : events) {
				if (event->time <= 0) {
					const int eventId = event->getId();
					if (eventListeners.find(eventId) != eventListeners.end()) {
						for (auto& it : eventListeners[eventId]) {
							it->invoke(event);
						}
					}
					eventsToRemove.push_back(event);
				} else {
					event->time -= dt / 1000.0;
				}
			}

			for (auto& event : eventsToRemove) {
				events.remove(event);
				delete event;
			}
			eventsToRemove.clear();
			addMutex.unlock();
		}
	};
}

#endif