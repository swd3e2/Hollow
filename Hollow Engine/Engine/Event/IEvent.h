#ifndef __I_EVENT_H__
#define __I_EVENT_H__
#pragma once
#include <chrono>
#include <stdint.h>

namespace Hollow {

    class IEvent
    {
    private:
        size_t m_EventTypeID;
        long float m_TimeCreated;
    public:
        IEvent(size_t typeID) : m_EventTypeID(typeID) 
        {
            // @TODO: create better time definition
            m_TimeCreated = 0;
        }

        virtual unsigned int GetEventTypeID() const = 0;
        virtual unsigned int GetTimeCreated() const = 0;    
    };

}

#endif