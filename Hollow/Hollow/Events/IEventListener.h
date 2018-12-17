#pragma once
#include <list>
#include "IEventDelegate.h"
#include "Hollow/ECS/EventHandler.h"
#include "Hollow/Platform.h"

namespace Hollow {
    class HOLLOW_API IEventListener
    {
    private:
        std::list<IEventDelegate*> m_RegisteredCallbacks;
    public:
		IEventListener();
		virtual ~IEventListener();

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

		void UnregisterAllEventsCallback();
    };
}