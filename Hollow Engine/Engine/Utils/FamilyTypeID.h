#pragma once

namespace Hollow { namespace Core { namespace Utils {

	template<class T>
	class FamilyTypeID
	{
		static unsigned int count;
	public:
		template<class U>
		static unsigned int Get()
		{
			static const unsigned int STATIC_TYPE_ID { count++ };
			return STATIC_TYPE_ID;
		}

		static unsigned int Get()
		{
			return count;
		}
	};

}}}