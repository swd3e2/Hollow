#pragma once

using eventId = size_t;

class IEvent
{
public:
	virtual eventId getEventId() = 0;
};