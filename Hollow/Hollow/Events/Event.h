#pragma once
#include "IEvent.h"
#include "Hollow/Utils/FamilyTypeID.h"

template<class T>
class Event : public IEvent
{
private:
	static const eventId id;
public:
	Event()
	{}

	virtual eventId getEventId() override { return id; }

	static eventId getStaticEventId() { return id; }
};

template<class T>
const eventId Event<T>::id = Hollow::Core::Utils::FamilyTypeID<IEvent>::template Get<T>();