#ifndef __MOVE_EVENT_H__
#define __MOVE_EVENT_H__
#include "Event.h"

namespace Hollow {

	class MoveEvent : public Event<MoveEvent>
	{
	public:
		MoveEvent() : moved(true), Event<MoveEvent>() {}
		bool moved;
	};
}

#endif