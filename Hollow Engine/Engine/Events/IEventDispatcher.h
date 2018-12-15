#ifndef __I_EVENT_DISPATCHER_H__
#define __I_EVENT_DISPATCHER_H__
#include <unordered_map>
#include "EventDelegate.h"

namespace Hollow {

	class IEventDispatcher
	{
	public:

		virtual ~IEventDispatcher()
		{}

		virtual void Dispatch(IEvent* event) = 0;

		virtual void AddEventCallback(IEventDelegate* const eventDelegate) = 0;

		virtual void RemoveEventCallback(IEventDelegate* eventDelegate) = 0;

		virtual inline size_t GetEventCallbackCount() const = 0;
	};

}

#endif