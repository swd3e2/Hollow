#ifndef __I_EVENT_LISTENER_H__
#define __I_EVENT_LISTENER_H__
#include <list>
#include "IEventDelegate.h"
#include "Engine/EventHandler.h"

namespace Hollow {

    class IEventListener
    {
    private:
        std::list<IEventDelegate*> m_RegisteredCallbacks;
    public:
        IEventListener() {}

		virtual ~IEventListener() { this->UnregisterAllEventsCallback(); }

        template<class E, class C>
        inline void RegisterEventCallback(void(C::*Callback)(const E* const))
        {
            EventDelegate<C, E>* eventDelegate = new EventDelegate<C, E>(static_cast<C*>(this), Callback);
            m_RegisteredCallbacks.push_back(eventDelegate);
			EventHandler::Get()->AddEventCallback<E>(eventDelegate);
        }

        template<class E, class C>
        inline void UnregisterEventCallback(void(C::*Callback)(const E* const))
        {
            EventDelegate* eventDelegate(static_cast<C*>(this), Callback);

            for (auto cb : this->m_RegisteredCallbacks)
            {
                if (cb->GetDelegateId() == eventDelegate->GetDelegateId())
                {
                    this->m_RegisteredCallbacks.remove_if(
                        [&](const EventDelegate* other)
                        {
                            return other->operator==(cb);
                        }
                    );

					EventHandler::Get()->AddEventCallback(EventDelegate);
                    break;
                }
            }
        }

        void UnregisterAllEventsCallback()
        {
            for (auto it : this->m_RegisteredCallbacks)
            {
				EventHandler::Get()->RemoveEventCallback(it);
            }
            this->m_RegisteredCallbacks.clear();
        }

    };

}

#endif