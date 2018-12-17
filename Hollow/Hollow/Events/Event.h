#pragma once
#include "Hollow/Utils/FamilyTypeID.h"
#include "Hollow/Platform.h"
#include "IEvent.h"

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