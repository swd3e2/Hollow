#include "System.h"

namespace Hollow {
	template<class T>
	const SystemTypeID System<T>::STATIC_SYSTEM_TYPE_ID = Core::Utils::FamilyTypeID<ISystem>::Get<System>();
}