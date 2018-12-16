#ifndef __I_EVENT_DELEGATE_H__
#define __I_EVENT_DELEGATE_H__
#include "IEvent.h"
#include "Platform.h"

namespace Hollow {
	class HOLLOW_API IEventDelegate
	{
	public:
		virtual inline void invoke(const IEvent* const e) = 0;

		virtual inline size_t GetDelegateId() const = 0;

		virtual inline uint64_t GetStaticEventTypeId() const = 0;

		virtual bool operator==(const IEventDelegate* other) const = 0;

		virtual IEventDelegate* clone() = 0;
	};
}

#endif