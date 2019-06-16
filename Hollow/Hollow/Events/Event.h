#pragma once

#ifndef HW_EVENT_H
#define HW_EVENT_H

#include "IEvent.h"
#include <typeinfo>

namespace Hollow {
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
	const eventId Event<T>::id = typeid(T).hash_code();
}


#endif