#pragma once
#include "Hollow/Platform.h"
#include "IEvent.h"

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