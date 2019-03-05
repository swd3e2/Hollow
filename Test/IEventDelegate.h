#pragma once
#include "IEvent.h"
#include "IEventListener.h"

class IEventDelegate
{
private:
public:
	eventId _eventId = -1;
	virtual void invoke(IEvent* event) = 0;

	eventId getSubscribedEventId()
	{
		return _eventId;
	}
};