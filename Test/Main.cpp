#include <iostream>
#include "Hollow/Containers/vector.h"
#include "Hollow/Containers/pair.h"

typedef size_t EntityID;
static Hollow::Containers::Vector<Hollow::Containers::Pair<EntityID, void*>, 1000> entityTable;

class A {};

int main()
{
	A* k = new A();
	for (int i = 0; i < 2000; i++)
		entityTable.push_back(Hollow::Containers::Pair<EntityID, void*>(1, (void*)k));

	A** table;
	table = (A**)new A[10];
	table[0] = new A();

	delete table;

	std::cin.get();
	return 0;
}

