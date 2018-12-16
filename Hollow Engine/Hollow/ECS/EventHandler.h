#ifndef __EVENT_HANDLER_H__
#define __EVENT_HANDLER_H__
#include "Events/EventDispatcher.h"
#include "Memory/Allocators/LinearAllocator.h"
#include "Memory/MemoryUser.h"
#include <unordered_map>
#include <list>
#include "Common/Log.h"
#include "Platform.h"
#include <new>
 #define ECS_EVENT_MEMORY_BUFFER_SIZE		4194304
namespace Hollow {

    using EventTypeID = size_t;

    class HOLLOW_API EventHandler : public Core::Memory::MemoryUser
    {
    private:
		static EventHandler*								instance;
        std::unordered_map<EventTypeID, IEventDispatcher*>  m_EventDispatcherMap;
        std::vector<IEvent*>                                m_EventStorage;
        Core::Memory::LinearAllocator*                      m_EventAllocator;
    private:
        EventHandler(const EventHandler&) = delete;
		EventHandler& operator=(EventHandler&) = delete;
	public:
        template<class E>
        inline void AddEventCallback(IEventDelegate* const eventDelegate)
        {
            EventTypeID ETID = E::STATIC_EVENT_TYPE_ID;
            auto it = this->m_EventDispatcherMap.find(ETID);
            if (it == this->m_EventDispatcherMap.end())
            {
				this->m_EventDispatcherMap[ETID] = new EventDispatcher<E>();
     			this->m_EventDispatcherMap[ETID]->AddEventCallback(eventDelegate);
            } else {
                this->m_EventDispatcherMap[ETID]->AddEventCallback(eventDelegate);
            }
        }

        inline void RemoveEventCallback(IEventDelegate* eventDelegate)
        {
            auto typeID = eventDelegate->GetStaticEventTypeId();
			std::unordered_map<EventTypeID, IEventDispatcher*>::const_iterator it = this->m_EventDispatcherMap.find(typeID);
            if (it != this->m_EventDispatcherMap.end())
            {
                this->m_EventDispatcherMap[typeID]->RemoveEventCallback(eventDelegate);
            }
        }
        EventHandler()
        {
			this->instance = this;
            // allocate memory from global memory manager
            this->m_EventAllocator = new Core::Memory::LinearAllocator(allocate(ECS_EVENT_MEMORY_BUFFER_SIZE, "EventHandler"), ECS_EVENT_MEMORY_BUFFER_SIZE);
            this->m_EventStorage.reserve(1024);
        }

        ~EventHandler()
        {
            for (std::unordered_map<EventTypeID, IEventDispatcher*>::iterator it = this->m_EventDispatcherMap.begin(); it != this->m_EventDispatcherMap.end(); ++it)
            {
                delete (*it).second;
                (*it).second = nullptr;
            }

            this->m_EventDispatcherMap.clear();

            // Release allocated memory
            free((void*)this->m_EventAllocator->GetMemoryFirstAddress());

            delete this->m_EventAllocator;
            this->m_EventAllocator = nullptr;
        }

        // clear buffer, that is, simply reset index buffer
		inline void ClearEventBuffer()
		{
			this->m_EventAllocator->clear();
			this->m_EventStorage.clear();
		}
	
		inline void ClearEventDispatcher()
		{
			this->m_EventDispatcherMap.clear();
		}
	
		template<class E, class... ARGS>
		void Send(ARGS&&... eventArgs)
		{
			// check if type of object is trivially copyable
			static_assert(std::is_trivially_copyable<E>::value, "Event is not trivially copyable.");
	
	
			// allocate memory to store event data
			void* pMem = this->m_EventAllocator->allocate(sizeof(E), alignof(E));

			// add new event to buffer and event storage
			if (pMem != nullptr)
			{
				this->m_EventStorage.push_back(new (pMem)E(std::forward<ARGS>(eventArgs)...));
			}
			else
			{
                Hollow::Log::GetCoreLogger()->critical("EventHandler: buffer is out of space!");
			}
		}
	
		// dispatches all stores events and clears buffer
		void DispatchEvents()
		{
			size_t lastIndex = this->m_EventStorage.size();
			size_t thisIndex = 0;

			while (thisIndex < lastIndex)
			{
				auto event = this->m_EventStorage[thisIndex++];
				if (event == nullptr) {
                    Hollow::Log::GetCoreLogger()->critical("EventHandler: Skip corrupted event with type {}", event->GetEventTypeID());
					continue;
				}

				auto it = this->m_EventDispatcherMap.find(event->GetEventTypeID());
				if (it == this->m_EventDispatcherMap.end())
					continue;
	
				it->second->Dispatch(event);

				// update last index, after dispatch operation there could be new events
				lastIndex = this->m_EventStorage.size();
			}
			
			// reset
			ClearEventBuffer();
		}

		static inline EventHandler* Get() { return instance; }
    };

}

#endif