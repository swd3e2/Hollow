#pragma once

#ifndef HW_I_EVENT_H
#define HW_I_EVENT_H

namespace Hollow {
	using eventId = size_t;

	class IEvent
	{
	public:
		virtual eventId getId() = 0;
	};
}

#endif