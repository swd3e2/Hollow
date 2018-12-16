#include "Component.h"

namespace Hollow {
	template<class T>
	const size_t Component<T>::STATIC_COMPONENT_TYPE_ID = Core::Utils::FamilyTypeID<IComponent>::Get<T>();
}