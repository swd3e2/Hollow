#include "Entity.h"

namespace Hollow {
	template<class E>
	const size_t Entity<E>::STATIC_ENTITY_TYPE_ID = Core::Utils::FamilyTypeID<IEntity>::Get<E>();
}