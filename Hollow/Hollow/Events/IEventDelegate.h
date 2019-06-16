#pragma once

#ifndef HW_I_EVENT_DELEGATE_H
#define HW_I_EVENT_DELEGATE_H

#include "IEvent.h"
#include "IEventListener.h"

namespace Hollow {
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
}

#endif