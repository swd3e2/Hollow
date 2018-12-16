#ifndef __EVENT_DELEGATE_H__
#define __EVENT_DELEGATE_H_
#pragma once_
#include "IEvent.h"
#include "IEventDelegate.h"

namespace Hollow {

	template<class Class, class EventType>
	class EventDelegate : public IEventDelegate
	{
		typedef void(Class::*Callback)(const EventType* const);

		Class* m_Receiver;
		Callback m_Callback;

	public:

		EventDelegate(Class* receiver, Callback& callbackFunction) :
			m_Receiver(receiver),
			m_Callback(callbackFunction)
		{}

		virtual IEventDelegate* clone() override
		{
			return new EventDelegate(this->m_Receiver, this->m_Callback);
		}

		virtual inline void invoke(const IEvent* const e) override
		{
			(m_Receiver->*m_Callback)(reinterpret_cast<const EventType* const>(e));
		}

		virtual inline size_t GetDelegateId() const override
		{
			static const size_t DELEGATE_ID{ typeid(Class).hash_code() ^ typeid(Callback).hash_code() };
			return DELEGATE_ID;
		}


		virtual inline uint64_t GetStaticEventTypeId() const override
		{
			static const uint64_t SEID{ EventType::STATIC_EVENT_TYPE_ID };
			return SEID;
		}

		virtual bool operator==(const IEventDelegate* other) const override
		{
			if (this->GetDelegateId() != other->GetDelegateId() || other == nullptr)
				return false;

			EventDelegate* delegate = (EventDelegate*)other;
			return ((this->m_Callback == delegate->m_Callback) && (this->m_Receiver == delegate->m_Receiver));
		}

	};

}

#endif