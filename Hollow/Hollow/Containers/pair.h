#pragma once

namespace Hollow { namespace Containers {

	template<class T, class U>
	class Pair
	{
	public:
		T first;
		U second;
	public:
		Pair(T first = 0, U second = 0) : first(first), second(second) {}
	};

} }
