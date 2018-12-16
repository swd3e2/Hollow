#ifndef __EVENT_H__
#define __EVENT_H__
#include "IEvent.h"
#include "Utils/FamilyTypeID.h"
#include "Platform.h"

namespace Hollow {
    template<typename E>
    class HOLLOW_API Event : public IEvent
    {
	public:
		static const size_t STATIC_EVENT_TYPE_ID;
    public:
        Event() : IEvent(STATIC_EVENT_TYPE_ID) {}
    };

}

#endif