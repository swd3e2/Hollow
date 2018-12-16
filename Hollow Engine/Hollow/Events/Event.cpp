#include "Event.h"

namespace Hollow {
	template<typename E>
	const size_t Event<E>::STATIC_EVENT_TYPE_ID = Core::Utils::FamilyTypeID<IEvent>::Get<E>();
}