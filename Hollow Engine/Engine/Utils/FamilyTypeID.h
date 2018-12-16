#pragma once

namespace Hollow { namespace Core { namespace Utils {

	template<class T>
	class FamilyTypeID
	{
		static size_t count;
	public:
		template<class U>
		static size_t Get()
		{
			static const size_t STATIC_TYPE_ID { count++ };
			return STATIC_TYPE_ID;
		}

		static size_t Get()
		{
			return count;
		}
	};

}}}