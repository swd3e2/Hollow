#pragma once
#include <vector>
#include "IEvent.h"
#include "IEventListener.h"
#include <unordered_map>
#include "EventDelegate.h"
#include <vector>
class EventSystem
{
private:
	std::vector<IEventDelegate*>  eventListeners;
	std::vector<IEvent*> events;
public:
	template<class T>
	void addEventListener(IEventListener* listener, void (T::*func)(IEvent*), eventId id)
	{
		eventListeners.push_back((IEventDelegate*)new EventDelegate<T>(listener, func, id));
	}

	void addEventListener(IEventDelegate* delegate)
	{
		eventListeners.push_back(delegate);
	}

	void addEvent(IEvent* event)
	{
		events.push_back(event);
	}

	void dispatch()
	{
		for (auto& ev : events) {
			for (auto& it : eventListeners) {
				if (ev->getEventId() == it->getSubscribedEventId()) {
					it->invoke(ev);
				}
			}
		}
		events.clear();
	}
};