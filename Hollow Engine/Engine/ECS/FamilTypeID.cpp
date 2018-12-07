#include "FamilyTypeID.h"



namespace Hollow { namespace Core { namespace Utils {
	class MyComponent;

	unsigned int FamilyTypeID<MyComponent>::count = 0;

	template class FamilyTypeID<MyComponent>;
}}}