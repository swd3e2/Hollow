#pragma once
#include <unordered_map>




namespace Hollow { namespace Containers {

	template<typename T, typename U>
	class hash_map
	{
	private:
		struct hash_value
		{

		};
	public:
		std::unordered_map<T, U> map;
		T* first;
		U* second;
	};

}}