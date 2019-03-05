#pragma once
#include "IEvent.h"
#include "ids.h"

template<class T>
class Event : public IEvent
{
private:
	static eventId id;
public:
	Event()
	{
		id = ids<IEvent>::template getId<T>();
	}

	virtual eventId getEventId() override { return id; }

	static eventId getStaticEventId() { return id; }
};

template<class T> eventId Event<T>::id;