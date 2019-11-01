#pragma once

#ifndef HW_I_EVENT_H
#define HW_I_EVENT_H

namespace Hollow {
	using eventId = size_t;

	class IEvent
	{
	private:
		friend class EventSystem;
		double time;
	public:
		virtual eventId getId() = 0;
	};
}

#endif