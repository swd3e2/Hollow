#pragma once
#include <vector>
#include "EventDelegate.h"
#include "Event.h"
#include "IEventListener.h"

template<class EventType>
class EventDispatcher
{
private:
	std::vector<IEventDelegate*>  eventDelegates;
public:
	void addEventDispatcher(EventDispatcher<IEventListener, EventType>* delegate)
	{
		for (auto& it : eventDelegates)
		{
			if (it->)
		}
	}

	void dispatch(IEvent* event)
	{
		for (auto& it : eventDelegates)
			it->invoke(event);
	}
};