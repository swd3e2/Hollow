#pragma once
#include "FamilyTypeID.h"

namespace ECS {
	class IEntity;
	class IComponent;
	class ISystem;

	namespace util {
		namespace Internal {

			u64 FamilyTypeID<IEntity>::s_count = 0u;
			u64 FamilyTypeID<IComponent>::s_count = 0u;
			u64 FamilyTypeID<ISystem>::s_count = 0u;

			template class FamilyTypeID<IEntity>;
			template class FamilyTypeID<IComponent>;
			template class FamilyTypeID<ISystem>;

		}
	}

} // namespace ECS::util::Internal