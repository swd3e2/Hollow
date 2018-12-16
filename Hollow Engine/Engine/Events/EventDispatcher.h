#ifndef __EVENT_DISPATCHER_H__
#define __EVENT_DISPATCHER_H__
#pragma once
#include "IEventDispatcher.h"
#include "IEventDelegate.h"

namespace Hollow {

	template<class EventType>
	class EventDispatcher : public IEventDispatcher
	{
	private:
		std::list<IEventDelegate*>	m_EventDeleagteList;
		std::list<IEventDelegate*>	m_PendingRemoveDelegates;
		bool						m_Locked;
	public:
		EventDispatcher() : m_Locked(false) {}

		virtual ~EventDispatcher()
		{
			m_EventDeleagteList.clear();
			m_PendingRemoveDelegates.clear();
		}
 
		virtual void Dispatch(IEvent* event) override
		{
			this->m_Locked = true;
			{
				if (this->m_PendingRemoveDelegates.empty() == false)
				{
					for (auto ec : this->m_PendingRemoveDelegates)
					{
						auto result = std::find_if(this->m_EventDeleagteList.begin(), this->m_EventDeleagteList.end(),
						[&](const IEventDelegate* other)
						{
							return other->operator==(ec);
						});

						if (result != this->m_EventDeleagteList.end())
						{
							IEventDelegate* ptrMem = (IEventDelegate*)(result);
							this->m_EventDeleagteList.erase(result);
							delete ptrMem;
							ptrMem = nullptr;
						}
					}
				}
				this->m_PendingRemoveDelegates.clear();
			}

			for (auto ec : this->m_EventDeleagteList)
			{
				assert(ec != nullptr && "Invalid event callback.");
				ec->invoke(event);
			}
			this->m_Locked = false;
		}

		virtual void AddEventCallback(IEventDelegate* const eventDelegate) override
		{
			auto result = std::find_if(this->m_PendingRemoveDelegates.begin(), this->m_PendingRemoveDelegates.end(),
				[&](const IEventDelegate* other)
				{
					return other->operator==(eventDelegate);
				});
			if (result != this->m_PendingRemoveDelegates.end())
			{
				this->m_PendingRemoveDelegates.erase(result);
				return;
			}
			this->m_EventDeleagteList.push_back(eventDelegate);
		}

		virtual void RemoveEventCallback(IEventDelegate* eventDelegate) override
		{ 
			if (this->m_Locked == false)
			{
				auto result = std::find_if(this->m_EventCallbacks.begin(), this->m_EventCallbacks.end(),
					[&](const IEventDelegate* other)
					{
						return other->operator==(eventDelegate);
					});

				if (result != this->m_EventCallbacks.end())
				{
					IEventDelegate* ptrMem = (IEventDelegate*)(*result);

					this->m_EventCallbacks.erase(result);

					delete ptrMem;
					ptrMem = nullptr;
				}
			}
			else
			{
				auto result = std::find_if(this->m_EventCallbacks.begin(), this->m_EventCallbacks.end(),
					[&](const IEventDelegate* other)
					{
						return other->operator==(eventDelegate);
					});

				assert(result != this->m_EventCallbacks.end() && "");
				this->m_PendingRemoveDelegates.push_back((*result));
			}
		}

		virtual inline size_t GetEventCallbackCount() override { return this->m_EventDeleagteList.size(); };
	};

}

#endif