#ifndef __EVENT_H__
#define __EVENT_H__
#include "IEvent.h"
#include "Engine/Utils/FamilyTypeID.h"

namespace Hollow {

    template<typename E>
    class Event : public IEvent
    {
    static const size_t STATIC_EVENT_TYPE_ID;
    public:
        Event() : IEvent() {}
    };

    template<typename E>
    const size_t Event<E>::STATIC_EVENT_TYPE_ID = Utils::FamilyTypeID<E>::Get<E>();
}

#endif