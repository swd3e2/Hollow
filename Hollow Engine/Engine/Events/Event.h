#ifndef __EVENT_H__
#define __EVENT_H__
#include "IEvent.h"
#include "Engine/Utils/FamilyTypeID.h"

namespace Hollow {

    template<typename E>
    class Event : public IEvent
    {
	public:
		static const size_t STATIC_EVENT_TYPE_ID;
    public:
        Event() : IEvent(STATIC_EVENT_TYPE_ID) {}
    };

    template<typename E>
    const size_t Event<E>::STATIC_EVENT_TYPE_ID = Core::Utils::FamilyTypeID<IEvent>::Get<E>();
}

#endif