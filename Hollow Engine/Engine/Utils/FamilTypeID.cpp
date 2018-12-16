#include "FamilyTypeID.h"

namespace Hollow {

	class IEntity;
	class IComponent;
	class ISystem;
	class IEvent;

	namespace Core { namespace Utils {

		size_t FamilyTypeID<IEntity>::count = 0;
		size_t FamilyTypeID<IComponent>::count = 0;
		size_t FamilyTypeID<ISystem>::count = 0;
		size_t FamilyTypeID<IEvent>::count = 0;

		template class FamilyTypeID<IEntity>;
		template class FamilyTypeID<IComponent>;
		template class FamilyTypeID<ISystem>;
		template class FamilyTypeID<IEvent>;
	}}
}