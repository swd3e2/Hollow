#include "FamilyTypeID.h"



namespace Hollow { 
	class IEntity;
	class IComponent;
	class ISystem;

	namespace Core { namespace Utils {

		unsigned int FamilyTypeID<IEntity>::count = 0;
		unsigned int FamilyTypeID<IComponent>::count = 0;
		unsigned int FamilyTypeID<ISystem>::count = 0;

		template class FamilyTypeID<IEntity>;
		template class FamilyTypeID<IComponent>;
		template class FamilyTypeID<ISystem>;

	}}
}