#include <iostream>
#include "Hollow/Containers/vector.h"
#include "Hollow/Containers/pair.h"

typedef size_t EntityID;
static Hollow::Containers::Vector<Hollow::Containers::Pair<EntityID, void*>> entityTable;

class A {};

int main()
{
	A* k = new A();

	entityTable.push_back(Hollow::Containers::Pair<EntityID, void*>(1, (void*)k));
	return 0;
}

