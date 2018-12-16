#ifndef __I_EVENT_H__
#define __I_EVENT_H__
#include <chrono>
#include <stdint.h>

namespace Hollow {

    class IEvent
    {
    private:
        size_t m_EventTypeID;
        long long m_TimeCreated;
    public:
        IEvent(size_t typeID)
        {
			m_EventTypeID = typeID;
            // @TODO: create better time definition
            m_TimeCreated = 0;
        }

		inline size_t GetEventTypeID() { return this->m_EventTypeID; }
        inline long long GetTimeCreated() { return this->m_TimeCreated; }
    };

}

#endif