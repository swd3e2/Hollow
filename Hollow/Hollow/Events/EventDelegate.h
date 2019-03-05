#pragma once
#include "IEventDelegate.h"

template<class T>
class EventDelegate : public IEventDelegate
{
private:
	void(T::*func)(IEvent*);
	IEventListener* listener;
public:
	EventDelegate(IEventListener* listener, void(T::*func)(IEvent*), eventId id)
	{
		this->func = func;
		this->listener = listener;
		this->_eventId = id;
	}

	virtual void invoke(IEvent* event) override
	{
		(((T*)listener)->*func)(event);
	}
};